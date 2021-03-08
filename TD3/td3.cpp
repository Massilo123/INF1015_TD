/**
* Fichier contenant l'implémentation des fonctions pour manipuler une collection de films, ainsi qu'un programme qui teste ces fonctions sur un exemple lu depuis un fichier.
* \file   td3.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   6 mars 2021
* Créé le 2 février 2021
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>

#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

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

shared_ptr<Acteur> ListeFilms::rechercherActeur(const string& nom) const
{
	// On parcourt tous les films de la liste.

	span<Film*> spanFilms(elements, size_t(nElements));
	for (Film* film : spanFilms)
	{
		// Pour chaque film, on parcourt tous ses acteurs.
		span<shared_ptr<Acteur>> spanActeurs(film->acteurs.obtenirElements(), size_t(film->acteurs.obtenirNElements()));
		for (shared_ptr<Acteur> acteurCourant : spanActeurs)
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

shared_ptr<Acteur> lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	// Lecture des données sur l'acteur depuis le fichier.
	string nom = lireString(fichier);
	int anneeNaissance = lireUint16(fichier);
	char sexe = lireUint8(fichier);

	// On vérifie l'existance de l'acteur lu.
	shared_ptr<Acteur> resultat = listeFilms.rechercherActeur(nom);

	// Si l'acteur lu n'existait pas déjà :
	if (resultat == nullptr)
	{
		// On crée un pointeur vers ce nouvel acteur.
		Acteur acteur = {};
		acteur.nom = nom;
		acteur.anneeNaissance = anneeNaissance;
		acteur.sexe = sexe;

		resultat = make_shared<Acteur>(acteur);
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

	film.acteurs = ListeActeurs();

	Film* filmPt = new Film(film);
	cout << "Film crée : " << filmPt->titre << endl;

	// Pour tous les acteurs du film :
	for (int i : range(nActeurs)) 
	{
		filmPt->acteurs.ajouterElement(lireActeur(fichier, listeFilms));
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

	capacite = 0;
	nElements = 0;
	delete[] elements;
	elements = new Film * [0];

	// Pour tous les films de la liste :
	for (int i : range(nFilms))
	{
		ajouterFilm(lireFilm(fichier, *this)); //TODO: Ajouter le film à la liste.
	}
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs. Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.

void detruireFilm(ListeFilms& listeFilms, Film* film)
{
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

// Permet d'afficher un acteur.
ostream& operator<< (ostream& o, const Acteur& acteur)
{
	return o << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ses acteurs (en utilisant la fonction afficherActeur ci-dessus).

ostream& operator<< (ostream& o, const Film& film)
{
	o << "\n\nTitre : " << film.titre << endl;
	o << "  Réalisateur : " << film.realisateur << endl;
	o << "  Année de sortie : " << film.anneeSortie << endl;
	o << "  Recette : " << film.recette << "M$" << endl;
	o << "Casting : " << endl;

	// On affiche tous les acteurs qui jouent dans le film.
	span<shared_ptr<Acteur>> spanActeurs(film.acteurs.obtenirElements(), size_t(film.acteurs.obtenirNElements()));
	for (shared_ptr<Acteur> acteur : spanActeurs)
	{
		o << *acteur;
	}

	return o;
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
		cout << *film;
		cout << ligneDeSeparation;
	}
}

// Renvoie le nombre de films contenus dans la liste.
int ListeFilms::obtenirNElements() const
{
	return nElements;
}

// Renvoie un pointeur vers le film de rang "id" dans la liste.
Film* ListeFilms::operator[](int id) const
{
	return elements[id];
}

// Renvoie le premier film trouvé correspondant au critère spécifié.
Film* ListeFilms::trouverFilmSelonCritere(const function<bool(Film*)>& critere) const
{
	Film* result = nullptr;
	int i = 0;
	span<Film*> spanFilms(elements, size_t(nElements));
	// On applique la lamba expression à tous les films de la liste jusqu'à obtenir un résultat positif.
	while (i < capacite && result == nullptr) 
	{
		if (critere(spanFilms[i]))
		{
			result = spanFilms[i];
		}
		i++;
	}
	return result;
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

	cout << ligneDeSeparation << "Recherche du film ayant 955M$ de recette :\n" << endl;
	cout << *listeFilms.trouverFilmSelonCritere([](Film* film) {return film->recette == 955;});
	
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.

	cout << *listeFilms[0];
	
	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.

	listeFilms.afficher();
	
	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.

	shared_ptr<Acteur> benedictCumberbatch = listeFilms.rechercherActeur("Benedict Cumberbatch");
	benedictCumberbatch->anneeNaissance = 1976;

	// Création du film Skylien :
	cout << ligneDeSeparation << "Création du film Skylien :\n" << endl;

	Film skylien = *listeFilms[0]; // On commence par copier le film Alien.
	skylien.titre = "Skylien"; // On modifie le titre.
	skylien.acteurs.obtenirElements()[0] = listeFilms[1]->acteurs.obtenirElements()[0]; // On remplace le premier acteur.
	skylien.acteurs.obtenirElements()[0]->nom = "Daniel Wroughton Craig"; // On modifie le nom du premier acteur.
	cout << skylien << *listeFilms[0] << *listeFilms[1];
	
	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.

	detruireFilm(listeFilms, listeFilms[0]);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.

	listeFilms.afficher();
	
	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.


	//TODO: Détruire tout avant de terminer le programme.  L'objet verifierFuitesAllocations devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.

	listeFilms.detruire();


	cout << ligneDeSeparation << "Tests sur une liste de points :\n" << endl;

	struct Point { double x, y; };
	Liste<Point> listePoints;
	listePoints.ajouterElement(make_shared<Point>(Point{ 1.2, 3.4 }));
	cout << listePoints.obtenirElements()[0]->x << " ; " << listePoints.obtenirElements()[0]->y << endl;


	cout << ligneDeSeparation << "Tests sur des listes de textes :\n" << endl;

	shared_ptr<string> texte1 = make_shared<string>("bonjour");
	shared_ptr<string> texte2 = make_shared<string>("au revoir");
	shared_ptr<string> texte3 = make_shared<string>("hello");

	Liste<string> listeTextes; // On crée une liste de textes.

	listeTextes.ajouterElement(texte1); // On y ajoute deux textes.
	listeTextes.ajouterElement(texte2);

	Liste<string> listeTextes2 = listeTextes; // On effectue une copie.
	
	listeTextes.obtenirElements()[0] = texte3; // On remplace le premier texte de la première liste.
	*listeTextes.obtenirElements()[1] = "goodbye"; // On modifie le deuxième texte de la première liste.
	
	cout << *listeTextes.obtenirElements()[0] << endl;
	cout << *listeTextes.obtenirElements()[1] << endl;
	cout << *listeTextes2.obtenirElements()[0] << endl;
	cout << *listeTextes2.obtenirElements()[1] << endl;
}
