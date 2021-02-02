/// Verification des fuites de mémoire.
/// Utilise un unordered_map pour conserver toutes les allocations, ceci a évidemment un impact sur la vitesse d'exécution, donc on ne l'utilise normalement pas sur un code final mais plutôt durant la vérification d'un programme.
/// \author Francois-R.Boyer@PolyMtl.ca
/// \version 2021-01
/// \since   2020-04

#include "verification_allocation.hpp"
#include <iostream>
#include <unordered_map>
#include <cassert>  // Peut-être utiliser gsl Expects à la place?
#include <limits>
#include <string>
#include <cstring>
using namespace std;

// Configuration si on veut afficher les erreur à la console et si on veut terminer l'exécution sur un erreur de désallocation.  Définir ou non les macros suivantes.
#define AFFICHER_ERREUR_DELETE
#define TERMINATE_SUR_ERREUR_DELETE

namespace bibliotheque_cours {

bool desactive_terminate_sur_erreur_delete = false;

SorteErreurDelete derniere_erreur_delete = SorteErreurDelete::no_error;
const char* get_message_erreur_delete() {
	static const char* messages[] = {
		"Pas d'erreur de delete.",
		"Desallocation d'un bloc non alloue (ou deja desalloue).",
		"delete[] d'un pointeur alloue avec new sans [].",
		"delete sans [] d'un pointeur alloue avec new [].",
		"le programme a ecrit hors des bornes d'une allocation memoire",
		"Erreur inconnue, probablement une corruption de memoire."
	};
	if (unsigned(derniere_erreur_delete) > unsigned(SorteErreurDelete::last))
		return messages[unsigned(SorteErreurDelete::last) + 1];
	return messages[unsigned(derniere_erreur_delete)];
}

static const InfoBlocMemoire pas_de_bloc = { numeric_limits<size_t>::max(), false, nullptr, 0 };
static void lancer_erreur_delete(SorteErreurDelete erreur, const InfoBlocMemoire& info = pas_de_bloc) {
	derniere_erreur_delete = erreur;
#if defined(TERMINATE_SUR_ERREUR_DELETE) || defined(AFFICHER_ERREUR_DELETE)
	// On ne peut pas "throw" dans l'operator delete, donc on "terminate" s'il y a une erreur.
	string message = get_message_erreur_delete();
	if (&info != &pas_de_bloc) {
		message += " Bloc de " + to_string(info.taille) + " octets, new" +  (info.est_tableau ? "[] " : " ");
		if (info.nom_fichier)
			message += ", ligne '"s + info.nom_fichier + "':" + to_string(info.ligne_fichier);
	}
	cerr << "Erreur verification allocation: " << message << endl;
#endif
#ifdef TERMINATE_SUR_ERREUR_DELETE
	// Voir le message d'erreur affiché sur cerr ou le contenu de la variable 'message'.
	// Voir aussi la "Pile des appels" dans le débogueur pour trouver à partir du haut de la liste le premier nom qui correspond à une fonction de votre programme, pour savoir quelle ligne a causée cette erreur.
	if (!desactive_terminate_sur_erreur_delete) terminate();
#endif
}

using TypeValeurVerificationCorruption = unsigned;
static constexpr TypeValeurVerificationCorruption valeur_verification_corruption = 0xDEADBEEF;
static constexpr size_t n_mots_verification_corruption = 4;
static constexpr size_t taille_verification_corruption = n_mots_verification_corruption * sizeof(valeur_verification_corruption);
static constexpr bool a_verification_corruption = taille_verification_corruption != 0;
static constexpr bool verification_contient_taille = taille_verification_corruption >= 2 * sizeof(valeur_verification_corruption);

//unordered_map<void*, InfoBlocMemoire> blocs_alloues;

size_t compteur_de_new = 0, compteur_de_delete = 0;

void remise_a_zero_compteurs_allocation() {
	compteur_de_new = 0;
	compteur_de_delete = 0;
	derniere_erreur_delete = SorteErreurDelete::no_error;
}
void remise_a_zero_verification() {
	blocs_alloues.clear();
	remise_a_zero_compteurs_allocation();
}

void activer_verification_allocation() { remise_a_zero_verification(); SansVerifierAllocations::est_actif = false; }
void desactiver_verification_allocation() { SansVerifierAllocations::est_actif = true; }

void remplir_bloc_verification_corruption_a(void* ptr, size_t sz)
{
	if constexpr (a_verification_corruption) {
		auto* p = reinterpret_cast<TypeValeurVerificationCorruption*>(ptr);
		if constexpr (verification_contient_taille) {
			p[0] = TypeValeurVerificationCorruption(sz);
			p[1] = p[0] ^ valeur_verification_corruption;
		}
		for (size_t i = verification_contient_taille ? 2 : 0; i < n_mots_verification_corruption; ++i)
			p[i] = valeur_verification_corruption;
	}
}
bool tester_bloc_verification_corruption_a(void* ptr)
{
	if constexpr (a_verification_corruption) {
		auto* p = reinterpret_cast<TypeValeurVerificationCorruption*>(ptr);
		if constexpr (verification_contient_taille) {
			if ((p[1] ^ p[0]) != valeur_verification_corruption)
				return false;
		}
		for (size_t i = verification_contient_taille ? 2 : 0; i < n_mots_verification_corruption; ++i)
			if (p[i] != valeur_verification_corruption)
				return false;
	}
	return true;
}


char* pointeur_octets(void* p) { return reinterpret_cast<char*>(p); }
//const char* pointeur_octets(const void* p) { return reinterpret_cast<const char*>(p); }

// Devrait être appelé uniquement lorsqu'on sait que le bloc n'est pas corrompu (l'assertion devrait se rendre compte d'un problème, mais ça n'est pas vérifiable dans un test).
size_t taille_allocation_selon_verification_corruption(void* ptr)
{
	assert(verification_contient_taille);
	auto* p = reinterpret_cast<TypeValeurVerificationCorruption*>(pointeur_octets(ptr) - taille_verification_corruption);
	assert((p[1] ^ p[0]) == valeur_verification_corruption);
	return p[0];
}

void mettre_verification_corruption_sur_allocation(void* ptr, size_t sz)
{
	if constexpr (a_verification_corruption) {
		remplir_bloc_verification_corruption_a(pointeur_octets(ptr) - taille_verification_corruption, sz);
		remplir_bloc_verification_corruption_a(pointeur_octets(ptr) + sz, sz);
	}
}
bool tester_verification_corruption_sur_allocation(void* ptr, size_t sz = numeric_limits<size_t>::max())
{
	if (!tester_bloc_verification_corruption_a(pointeur_octets(ptr) - taille_verification_corruption))
		return false;
	if (sz == numeric_limits<size_t>::max()) {
		if constexpr (verification_contient_taille)
			sz = taille_allocation_selon_verification_corruption(ptr);
		else
			// On ne peut pas vérifier le bloc à la fin car on ne sait pas la longueur de l'allocation.
			return true;
	}
	return tester_bloc_verification_corruption_a(pointeur_octets(ptr) + sz);
}


void* notre_operator_new(size_t sz, bool est_tableau, const char* nom_fichier = nullptr, int ligne_fichier = 0) {
	SansVerifierAllocations sva;
	// On ne supporte pas actuellement l'allocation de blocs plus gros de 4GB.
	if (sz > numeric_limits<TypeValeurVerificationCorruption>::max())
		throw std::runtime_error("Allocation trop grosse ou negative.");
	// On met la vérification de corruption partout, même pour les blocs "sans vérifier allocation", pour ne pas que les adresses soient différentes dans les deux cas.  "sans vérifier allocation" indique donc seulement de ne pas mettre les blocs dans le map des blocs_alloues.
	void* ptr = std::malloc(sz + 2 * taille_verification_corruption);
	if (ptr == nullptr)
		throw std::bad_alloc();
	ptr = pointeur_octets(ptr) + taille_verification_corruption;
	mettre_verification_corruption_sur_allocation(ptr, sz);
	memset(ptr, 0xCC, sz);  // Pour ne pas qu'on puisse supposer que l'allocation initialise à zéro.

	if (!sva.etait_deja_actif()) {
		// Attention que l'ajout aux blocs_alloues fait possiblement une allocation, et doit donc absolument être contrôlée pour ne pas faire une récursion infinie.
		blocs_alloues[ptr] = { sz, est_tableau, nom_fichier, ligne_fichier };
		compteur_de_new++;
	}

	return ptr;
}

void notre_operator_delete(void* ptr, bool est_tableau) noexcept {
	if (ptr == nullptr)
		return;
	SansVerifierAllocations sva;
	if (!sva.etait_deja_actif()) {
		auto it = blocs_alloues.find(ptr);
		if (it == blocs_alloues.end())
			return lancer_erreur_delete(SorteErreurDelete::not_allocated);
		
		if (it->second.est_tableau != est_tableau)
			return lancer_erreur_delete(est_tableau ? SorteErreurDelete::wrong_delete_array : SorteErreurDelete::wrong_delete_nonarray, it->second);
		
		if (!tester_verification_corruption_sur_allocation(ptr, it->second.taille))
			return lancer_erreur_delete(SorteErreurDelete::corruption, it->second);
		
		memset(ptr, 0xCC, it->second.taille);  // Pour ne pas qu'on puisse relire les données désallouées.
		blocs_alloues.erase(it);
		compteur_de_delete++;
	}
	else if (VerifierFuitesAllocations::est_dans_phase_apres_main()) {
		// Dans la phase après main, il peut y avoir des désallocations pour les "static". Le delete d'un bloc vérifié doit l'enlever même s'il est hors vérification, on ne donne juste pas d'erreur.
		if (auto it = blocs_alloues.find(ptr); it != blocs_alloues.end()) {
			blocs_alloues.erase(it);
			compteur_de_delete++;
		}
	}

	if (!tester_verification_corruption_sur_allocation(ptr))
		return lancer_erreur_delete(SorteErreurDelete::corruption);
	
	std::free(pointeur_octets(ptr) - taille_verification_corruption);
}

bool tous_les_new_ont_un_delete() {
	return blocs_alloues.empty();
}
void dump_blocs_alloues() {
	SansVerifierAllocations sva;
	for (const auto& [ptr, info] : blocs_alloues) {
		cout << ptr << " new" << (info.est_tableau ? "[] " : "   ") << info.taille << " octets";
		if (info.nom_fichier)
			cout << ", ligne '" << info.nom_fichier << "':" << info.ligne_fichier;
		cout << endl;
	}
}
void afficher_fuites() {
	if (tous_les_new_ont_un_delete())
		cout << endl << "Aucune fuite detectee." << endl;
	else {
		cout << endl << "Fuite detectee:" << endl;
		dump_blocs_alloues();
	}
}
bool tester_tous_blocs_alloues() {
	for (const auto& [ptr, info] : blocs_alloues)
		if (!tester_verification_corruption_sur_allocation(ptr, info.taille))
			return false;
	return true;
}

}

// Les operator new et delete doivent être hors du namespace.
#ifdef _MSC_VER
__pragma(warning(push)) __pragma(warning(disable:28251 28252 28253))
#endif
void* operator new  (size_t sz) { return bibliotheque_cours::notre_operator_new(sz, false); }
void* operator new[](size_t sz) { return bibliotheque_cours::notre_operator_new(sz, true);  }
void* operator new  (size_t sz, const char* nom_fichier, int ligne_fichier) { return bibliotheque_cours::notre_operator_new(sz, false, nom_fichier, ligne_fichier); }
void* operator new[](size_t sz, const char* nom_fichier, int ligne_fichier) { return bibliotheque_cours::notre_operator_new(sz, true, nom_fichier, ligne_fichier);  }

#ifdef _MSC_VER
__pragma(warning(pop))
#endif
void operator delete  (void* ptr) noexcept { bibliotheque_cours::notre_operator_delete(ptr, false); }
void operator delete[](void* ptr) noexcept { bibliotheque_cours::notre_operator_delete(ptr, true);  }
void operator delete  (void* ptr, size_t) noexcept { bibliotheque_cours::notre_operator_delete(ptr, false); }  // On pourrait passer le size à notre opérateur pour ajouter un test sur cette taille.
void operator delete[](void* ptr, size_t) noexcept { bibliotheque_cours::notre_operator_delete(ptr, true);  }
void operator delete  (void* ptr, const char*, int) noexcept { bibliotheque_cours::notre_operator_delete(ptr, false); }
void operator delete[](void* ptr, const char*, int) noexcept { bibliotheque_cours::notre_operator_delete(ptr, true);  }
