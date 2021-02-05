#pragma once
/// Verification des fuites de mémoire.
/// Utilise un unordered_map pour conserver toutes les allocations, ceci a évidemment un impact sur la vitesse d'exécution, donc on ne l'utilise normalement pas sur un code final mais plutôt durant la vérification d'un programme.
/// \author Francois-R.Boyer@PolyMtl.ca
/// \version 2021-01
/// \since   2020-04

#include <unordered_map>

#define VERIFICATION_ALLOCATION_INCLUS

namespace bibliotheque_cours {

struct InfoBlocMemoire {
	size_t taille;
	bool est_tableau;
	const char* nom_fichier; int ligne_fichier;
};

enum class SorteErreurDelete {
	no_error, not_allocated, wrong_delete_array, wrong_delete_nonarray, corruption, last = corruption
};

class SansVerifierAllocations {
public:
	SansVerifierAllocations() : deja_actif(est_actif) { est_actif = true; }
	~SansVerifierAllocations() { if (!deja_actif) est_actif = false; }
	bool etait_deja_actif() { return deja_actif; }

	friend void activer_verification_allocation();
	friend void desactiver_verification_allocation();

private:
	bool deja_actif;

	inline static bool est_actif = true;
};

const char* get_message_erreur_delete();
void remise_a_zero_compteurs_allocation();
void remise_a_zero_verification();
void activer_verification_allocation();
void desactiver_verification_allocation();
void afficher_fuites();

bool tous_les_new_ont_un_delete();
void dump_blocs_alloues();
bool tester_tous_blocs_alloues();

//void remplir_bloc_verification_corruption_a(void* ptr, size_t sz);
//bool tester_bloc_verification_corruption_a(void* ptr);
//char* pointeur_octets(void* p);
//const char* pointeur_octets(const void* p);
//void* notre_operator_new(size_t sz, bool est_tableau);
//void notre_operator_delete(void* ptr, bool est_tableau) noexcept;


/// Devrait être instaciée une seule fois au début du "main" pour vérifier les fuites du programme (excluant les allocations faites avant l'exécution du "main").
class VerifierFuitesAllocations {
public:
	VerifierFuitesAllocations() {
		static VerifierALaFin verifierALaFin;
		activer_verification_allocation();
	}
	// On désactive à la fin du main, pour ne pas qu'un delete hors main donne une erreur comme quoi il n'a pas de new associé, mais on affiche les fuites seulement plus tard.
	~VerifierFuitesAllocations() {
		est_phase_apres_main = true;
		desactiver_verification_allocation();
	}
	static bool est_dans_phase_apres_main() { return est_phase_apres_main; }

private:
	struct VerifierALaFin { ~VerifierALaFin() {
		afficher_fuites();
		VerifierFuitesAllocations::est_phase_apres_main = false;
	} };
	static inline bool est_phase_apres_main = false;
};

inline std::unordered_map<void*, InfoBlocMemoire> blocs_alloues;
extern SorteErreurDelete derniere_erreur_delete;
extern size_t compteur_de_new, compteur_de_delete;
extern bool desactive_terminate_sur_erreur_delete;

}

// Les operator new et delete doivent être hors du namespace.

//void* operator new  (size_t sz);
//void* operator new[](size_t sz);
//void operator delete  (void* ptr) noexcept;
//void operator delete[](void* ptr) noexcept;
