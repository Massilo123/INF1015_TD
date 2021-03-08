/**
* Fichier contenant la déclaration des structures pour manipuler une collection de films.
* \file   structures.hpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   6 mars 2021
* Créé le 2 février 2021
*/


#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>

#include "cppitertools/range.hpp"

using namespace std;
using namespace iter;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms
{
public:
	ListeFilms(); // Constructeur par défaut : initialise une liste vide avec une capacité de 0.

	void lire(const string& nomFichier); // Remplit la liste à partir du contenu d'un fichier.
	void detruire(); // Libère la mémoire allouée pour la liste de films.

	void afficher() const; // Affiche l'ensemble des films de la liste.

	void ajouterFilm(Film* film); // Ajoute un film à la liste, alloue de la mémoire supplémentaire si nécessaire.
	void retirerFilm(Film* film); // Retire un film de la liste, ne conserve pas l'ordre au sein de la liste.

	shared_ptr<Acteur> rechercherActeur(const string& nom) const; // Renvoie un pointeur vers un acteur ayant le nom indiqué, ou nullptr si cet acteur est introuvable dans la liste.

	// Accesseurs
	int obtenirNElements() const; // Renvoie le nombre de films contenus dans la liste.
	Film* operator[](int id) const; // Renvoie un pointeur vers le film de rang "id" dans la liste.

	Film* trouverFilmSelonCritere(const function<bool(Film*)>& critere) const; // Renvoie le premier film trouvé correspondant au critère spécifié.

private:
	int capacite; // Nombre de films pouvant être contenus dans la structure avec la mémoire allouée pour le tableau dynamique.
	int nElements; // Nombre de films contenus dans la structure.
	Film** elements; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Acteur.
};

template <typename T>
class Liste 
{
public:

	// Permet de construire une liste de capacité donnée.
	Liste(int capacite = 0)
	{
		capacite_ = capacite;
		nElements_ = 0;
		elements_ = make_unique<shared_ptr<T>[]>(capacite);
	}

	// Opérateur d'affectation : permet de copier une liste.
	Liste<T>& operator=(const Liste<T>& autre)
	{
		if (this != &autre)
		{
			capacite_ = autre.capacite_;
			nElements_ = autre.nElements_;
			elements_ = make_unique<shared_ptr<T>[]>(capacite_);

			for (int i : iter::range(capacite_))
			{
				elements_[i] = autre.elements_[i];
			}
		}

		return *this;
	}

	// Constructeur de copie.
	Liste(const Liste<T>& liste)
	{
		*this = liste;
	}

	// Accesseurs :
	int obtenirNElements() const
	{
		return nElements_;
	}

	shared_ptr<T>* obtenirElements() const
	{
		return elements_.get();
	}

	// Permet d'insérer un élément à la fin de la liste.
	void ajouterElement(shared_ptr<T> element)
	{
		// Si la capacité est invalide, on ré-initialise les attributs pour une capacité de 1.
		if (capacite_ <= 0)
		{
			capacite_ = 1;
			nElements_ = 0;
			elements_ = make_unique<shared_ptr<T>[]>(1);
		}

		// Si la capacité est insuffisante pour ajouter l'élément :
		if (capacite_ <= nElements_)
		{
			capacite_ = 2 * capacite_;

			// On alloue une liste dont la capacité est le double de la précédente.
			unique_ptr<shared_ptr<T>[]> newElements = make_unique<shared_ptr<T>[]>(capacite_);

			for (int i : range(nElements_))
			{
				newElements[i] = elements_[i];
			}

			elements_ = move(newElements);
		}

		// On ajoute l'élément à la première position libre.
		elements_[nElements_] = element;
		nElements_++;
	}

private:
	int capacite_, nElements_;
	unique_ptr<shared_ptr<T>[]> elements_;

};

using ListeActeurs = Liste<Acteur>;

struct Film {
	string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	// Constructeur par défaut.
	Film()
	{
		titre = "";
		realisateur = "";
		anneeSortie = 0;
		recette = 0;
		acteurs = ListeActeurs();
	}

	// Constructeur de copie.
	Film(const Film& film)
	{
		titre = film.titre;
		realisateur = film.realisateur;
		anneeSortie = film.anneeSortie;
		recette = film.recette;
		acteurs = film.acteurs;
	}
};

struct Acteur
{
	string nom = ""; int anneeNaissance = 0; char sexe = 'u';
};
