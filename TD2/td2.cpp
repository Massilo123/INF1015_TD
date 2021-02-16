/**
* Fichier contenant l'implémentation des fonctions pour manipuler une collection de films, ainsi qu'un programme qui teste ces fonctions sur un exemple lu depuis un fichier.
* \file   td2.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   10 février 2021
* Créé le 2 février 2021
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
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

//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.

void ListeFilms::ajouterFilm(Film* film)
{
	// Si la capacité est invalide, on ré-initialise les attributs pour une capacité de 1.
	if (capacite <= 0)
	{
		capacite = 1;
		nElements = 0;
		delete[] elements;
		elements = new Film * [1];
	}

	// Si la capacité est insuffisante pour ajouter l'élément :
	if (capacite <= nElements)
	{
		// On alloue un nouveau tableau, plus grand.
		Film** nouveauTableau = new Film * [2 * capacite];

		// On y copie le contenu de l'ancien.
		for (int i : range(capacite))
		{
			nouveauTableau[i] = elements[i];
		}

		// On élimine l'ancien tableau.
		delete[] elements;

		// On met à jour la structure.
		elements = nouveauTableau;
		capacite = 2 * capacite;
	}

	// On ajoute l'élément à la première position libre.
	elements[nElements] = film;
	nElements++;
}

//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.

void ListeFilms::retirerFilm(Film* film)
{
	bool filmTrouve = false;
	int i = 0;
	// Tant qu'on n'a pas trouvé le film à retirer, et qu'on n'a pas parcouru tous les films de la liste :
	while (i < nElements && !filmTrouve)
	{
		// On vérifie si le film courant est celui à retirer.
		if (elements[i] == film)
		{
			filmTrouve = true;
		}
		i++;
	}

	// Si on a trouvé le film à retirer :
	if (filmTrouve)
	{
		// On remplace le pointeur à l'emplacement trouvé par le dernier pointeur de la liste.
		elements[i - 1] = elements[nElements - 1];

		// On libère l'espace occupé précédemment par le dernier pointeur.
		elements[nElements - 1] = nullptr;
		nElements--;
	}
}

//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.

Acteur* ListeFilms::rechercherActeur(const string& nom) const
{
	// On parcourt tous les films de la liste.

	span<Film*> spanFilms(elements, size_t(nElements));
	for (Film* film : spanFilms)
	{
		// Pour chaque film, on parcourt tous ses acteurs.
		span<Acteur*> spanActeurs(film->acteurs.elements, size_t(film->acteurs.nElements));
		for (Acteur* acteurCourant : spanActeurs)
		{
			// Si l'acteur courant a le nom recherché, on retourne le pointeur vers cet acteur.
			if (acteurCourant->nom == nom)
			{
				return acteurCourant;
			}
		}
	}

	return nullptr;
}

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).

Acteur* lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	// Lecture des données sur l'acteur depuis le fichier.
	string nom = lireString(fichier);
	int anneeNaissance = lireUint16(fichier);
	char sexe = lireUint8(fichier);

	// On vérifie l'existance de l'acteur lu.
	Acteur* resultat = listeFilms.rechercherActeur(nom);

	// Si l'acteur lu n'existait pas déjà :
	if (resultat == nullptr)
	{
		// On crée un pointeur vers ce nouvel acteur.
		Acteur acteur = {};
		acteur.nom = nom;
		acteur.anneeNaissance = anneeNaissance;
		acteur.sexe = sexe;

		resultat = new Acteur(acteur);
		cout << "Acteur créé : " << acteur.nom << endl;
	}
	return resultat; //TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	// Lecture des données sur le film depuis le fichier.
	Film film = {};
	film.titre = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16(fichier);
	film.recette = lireUint16(fichier);

	int nActeurs = lireUint8(fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.

	ListeActeurs listeActeurs = {};
	listeActeurs.capacite = nActeurs;
	listeActeurs.nElements = 0;
	listeActeurs.elements = new Acteur * [nActeurs];

	film.acteurs = listeActeurs;

	Film* filmPt = new Film(film);
	cout << "Film crée : " << filmPt->titre << endl;

	// Pour tous les acteurs du film :
	for (int i : range(nActeurs)) 
	{
		filmPt->acteurs.elements[i] = lireActeur(fichier, listeFilms); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		filmPt->acteurs.nElements++;
		//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
		filmPt->acteurs.elements[i]->joueDans.ajouterFilm(filmPt);
	}
	return filmPt; //TODO: Retourner le pointeur vers le nouveau film.
}

ListeFilms::ListeFilms()
{
	// Par défaut, on initialise une liste vide, avec une capacité de 0.
	capacite = 0;
	nElements = 0;
	elements = new Film * [0];
}

void ListeFilms::lire(const string& nomFichier)
{
	// Lecture des données sur la liste depuis le fichier.
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nFilms = lireUint16(fichier);

	capacite = nFilms;
	nElements = 0;
	delete[] elements;
	elements = new Film * [nFilms]; // Allocation de la mémoire nécessaire.

	// Pour tous les films de la liste :
	for (int i : range(nFilms))
	{
		ajouterFilm(lireFilm(fichier, *this)); //TODO: Ajouter le film à la liste.
	}
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs. Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.

void detruireFilm(ListeFilms& listeFilms, Film* film)
{
	// Pour tous les acteurs qui jouent dans le film :
	span<Acteur*> spanActeurs(film->acteurs.elements, size_t(film->acteurs.nElements));
	for (Acteur* acteur : spanActeurs)
	{
		// On retire le film de la liste dans lesquels l'acteur joue.
		acteur->joueDans.retirerFilm(film);

		// Si l'acteur ne joue dans aucun autre film, on le détruit.
		if (acteur->joueDans.obtenirNElements() == 0)
		{
			cout << "Acteur détruit : " << acteur->nom << endl;
			acteur->joueDans.detruire();
			delete acteur;
		}
	}

	// On désalloue la mémoire pour le tableau dynamique d'acteurs.
	delete[] film->acteurs.elements;

	listeFilms.retirerFilm(film);

	// On détruit le film.
	cout << "Film détruit : " << film->titre << endl;
	delete film;
}

//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.

void ListeFilms::detruire()
{
	// On supprime l'élément en tête de liste jusqu'à obtenir une liste vide.
	while (nElements > 0)
	{
		detruireFilm(*this, elements[0]);
	}
	delete[] elements; // On libère la mémoire allouée pour le tableau dynamique de films.
}

void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ses acteurs (en utilisant la fonction afficherActeur ci-dessus).

void afficherFilm(const Film& film)
{
	cout << "\n\nTitre : " << film.titre << endl;
	cout << "  Réalisateur : " << film.realisateur << endl;
	cout << "  Année de sortie : " << film.anneeSortie << endl;
	cout << "  Recette : " << film.recette << "M$" << endl;
	cout << "Casting : " << endl;

	// On affiche tous les acteurs qui jouent dans le film.
	span<Acteur*> spanActeurs(film.acteurs.elements, size_t(film.acteurs.nElements));
	for (Acteur* acteur : spanActeurs)
	{
		afficherActeur(*acteur);
	}
}

void ListeFilms::afficher() const
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = "\n________________________________\n";
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	span<Film*> spanFilms(elements, size_t(nElements));
	for (Film* film : spanFilms)
	{
		//TODO: Afficher le film.
		afficherFilm(*film);
		cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur* acteur = listeFilms.rechercherActeur(nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		acteur->joueDans.afficher();
}

// Renvoie le nombre de films contenus dans la liste.
int ListeFilms::obtenirNElements() const
{
	return nElements;
}

// Renvoie un pointeur vers le film de rang "id" dans la liste.
Film* ListeFilms::obtenirFilm(int id) const
{
	return elements[id];
}

int main()
{
	#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).

	ListeFilms listeFilms = ListeFilms();
	listeFilms.lire("films.bin");
	
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.

	afficherFilm(*listeFilms.obtenirFilm(0));
	
	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.

	listeFilms.afficher();
	
	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.

	Acteur* benedictCumberbatch = listeFilms.rechercherActeur("Benedict Cumberbatch");
	benedictCumberbatch->anneeNaissance = 1976;
	
	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.

	afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");
	
	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.

	detruireFilm(listeFilms, listeFilms.obtenirFilm(0));

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.

	listeFilms.afficher();
	
	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	// On tente d'afficher la filmographie d'un acteur avec un nom incorrect.
	afficherFilmographieActeur(listeFilms, "Benedict Cumberbotch");

	//TODO: Détruire tout avant de terminer le programme.  L'objet verifierFuitesAllocations devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.

	listeFilms.detruire();
}
