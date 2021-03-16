/**
* Fichier contenant l'implémentation des fonctions pour manipuler une collection de films, ainsi qu'un programme qui teste ces fonctions sur un exemple lu depuis un fichier.
* \file   td4.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   16 mars 2021
* Créé le 9 mars 2021
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <iomanip>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).
using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

// Fonctions pour ajouter un Film à une ListeFilms.
//[
void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film*[nouvelleCapacite];
	
	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements_ sera zéro si elements_ est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}
	
	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}
//]

// Fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
//[
// On a juste fait une version const qui retourne un span non const.  C'est valide puisque c'est la struct qui est const et non ce qu'elle pointe.  Ça ne va peut-être pas bien dans l'idée qu'on ne devrait pas pouvoir modifier une liste const, mais il y aurais alors plusieurs fonctions à écrire en version const et non-const pour que ça fonctionne bien, et ce n'est pas le but du TD (il n'a pas encore vraiment de manière propre en C++ de définir les deux d'un coup).
span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}
//]

// Fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
//[

//NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions.
shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->obtenirActeurs().enSpan()) {
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}
//]

// Les fonctions pour lire le fichier et créer/allouer une ListeFilms.

shared_ptr<Acteur> lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = lireUint16 (fichier);
	acteur.sexe           = lireUint8  (fichier);

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(move(acteur));  // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->ecrireTitre(lireString(fichier));
	film->ecrireRealisateur(lireString(fichier));
	film->ecrireAnneeSortie(lireUint16 (fichier));
	film->ecrireRecette(lireUint16 (fichier));
	auto nActeurs = lireUint8 (fichier);
	film->obtenirActeurs() = ListeActeurs(nActeurs);  // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	cout << "Création Film " << film->lireTitre() << endl;

	for ([[maybe_unused]] auto i : range(nActeurs)) {  // On peut aussi mettre nElements_ avant et faire un span, comme on le faisait au TD précédent.
		film->obtenirActeurs().ajouter(lireActeur(fichier, listeFilms));
	}

	return film;
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nElements = lireUint16(fichier);

	ListeFilms listeFilms;
	for ([[maybe_unused]] int i : range(nElements)) { //NOTE: On ne peut pas faire un span simple avec ListeFilms::enSpan car la liste est vide et on ajoute des éléments à mesure.
		listeFilms.ajouterFilm(lireFilm(fichier, listeFilms));
	}
	
	return listeFilms;
}

// Fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
//NOTE: La bonne manière serait que la liste sache si elle possède, plutôt qu'on le dise au moment de la destruction, et que ceci soit le destructeur.  Mais ça aurait complexifié le TD2 de demander une solution de ce genre, d'où le fait qu'on a dit de le mettre dans une méthode.
void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film* film : enSpan())
			delete film;
	delete[] elements;
}
//]

// Pour que l'affichage de Film fonctionne avec <<, il faut aussi modifier l'affichage de l'acteur pour avoir un ostream; l'énoncé ne demande pas que ce soit un opérateur, mais tant qu'à y être...
ostream& operator<< (ostream& os, const Acteur& acteur)
{
	return os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}




/*Fonctions ajoutées pour le TD4*/


// Permet de transférer des films depuis une ListeFilms vers un vector<shared_ptr<Item>>.
void transfererListeFilms(vector<shared_ptr<Item>>& listeItems, ListeFilms& listeFilms)
{
	for (int i : range(listeFilms.size()))
	{
		listeItems.push_back(make_shared<Film>(*listeFilms[i]));
	}
}

// Permet de créer des Items à partir de données textuelles et de les ajouter à un vector<shared_ptr<Item>>.
void lireLivres(vector<shared_ptr<Item>>& listeItems, const string& nomFichier)
{
	ifstream livres(nomFichier);

	string titre, auteur = "";
	int anneeSortie, nCopiesVendues, nPages = 0;
	while (!ws(livres).eof())
	{
		livres >> quoted(titre);
		livres >> anneeSortie;
		livres >> quoted(auteur);
		livres >> nCopiesVendues;
		livres >> nPages;
		listeItems.push_back(make_shared<Livre>(Livre(titre, anneeSortie, auteur, nCopiesVendues, nPages)));
	}
}

// Permet d'afficher uniquement les informations propres aux Livres (utile pour afficher des FilmLivres).
ostream& Livre::afficherAux(ostream& o) const
{
	return o << "\nAuteur :\t\t" << auteur_ << "\nCopies vendues :\t" << nCopiesVendues_ << "M\nNombre de pages :\t" << nPages_ << endl;
}

// Affichage d'un Livre.
ostream& Livre::afficher(ostream& o) const
{
	return Livre::afficherAux(Item::afficher(o));
}

// Affichage d'un Film.
ostream& Film::afficher(ostream& o) const
{
	ostream& result = Item::afficher(o) << "\nRéalisateur :\t\t" << realisateur_ << "\nRecette :\t\t" << recette_ << "M$";
	result << "\nActeurs:" << endl;
	for (const shared_ptr<Acteur>& acteur : acteurs_.enSpan())
		result << *acteur;
	return result;
}

// Affichage d'un FilmLivre.
ostream& FilmLivre::afficher(ostream& o) const
{
	return Livre::afficherAux(Film::afficher(o));
}

// Surcharge de l'opérateur << pour afficher n'importe quel Item.
ostream& operator<<(ostream& o, const Item& item)
{
	return item.afficher(o);
}

// Surcharge de l'opérateur << pour afficher directement un vecteur d'Items.
ostream& operator<< (ostream& os, const vector<shared_ptr<Item>>& listeItems)
{
	static const string ligneDeSeparation = //[
		"\033[32m────────────────────────────────────────\033[0m\n";
	os << ligneDeSeparation;
	for (int i : range(listeItems.size()))
	{
		os << *listeItems[i].get() << ligneDeSeparation;
	}
	return os;
}

int main()
{
	#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	// On utilise la ListeFilms uniquement pour lire les données depuis le fichier .bin.
	ListeFilms listeFilms = creerListe("films.bin");

	// On crée une bibliothèque vers laquelle on transfère les films, puis on détruit la liste de films.
	vector<shared_ptr<Item>> listeItems(0);
	transfererListeFilms(listeItems, listeFilms);
	listeFilms.detruire(true);

	// On lit les données sur les livres et on les ajoute à la bibliothèque.
	lireLivres(listeItems, "livres.txt");

	// On construit un FilmLivre à partir du Film et du Livre "Le Hobbit", puis on l'ajoute à la bibliothèque.
	const Film& hobbitFilm = dynamic_cast<Film&>(*listeItems[4].get());
	const Livre& hobbitLivre = dynamic_cast<Livre&>(*listeItems[9].get());
	listeItems.push_back(make_shared<FilmLivre>(FilmLivre(hobbitFilm, hobbitLivre)));

	// On affiche la bibliothèque.
	cout << listeItems << endl;
}
