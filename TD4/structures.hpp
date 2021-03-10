/**
* Fichier contenant la déclaration des structures pour manipuler une collection de films. Basé sur le solutionnaire du TD3.
* \file   structures.hpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   9 mars 2021
* Créé le 9 mars 2021
*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	void detruire(bool possedeLesFilms = false);
	Film*& operator[] (int index) { return elements[index]; }
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template <typename T>
class Liste {
public:
	Liste() = default;
	explicit Liste(int capaciteInitiale) :  // explicit n'est pas matière à ce TD, mais c'est un cas où c'est bon de l'utiliser, pour ne pas qu'il construise implicitement une Liste à partir d'un entier, par exemple "maListe = 4;".
		capacite_(capaciteInitiale),
		elements_(make_unique<shared_ptr<T>[]>(capacite_))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite_(autre.nElements_),
		nElements_(autre.nElements_),
		elements_(make_unique<shared_ptr<T>[]>(nElements_))
	{
		for (int i = 0; i < nElements_; ++i)
			elements_[i] = autre.elements_[i];
	}
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simple avec unique_ptr...
	Liste<T>& operator= (Liste<T>&&) noexcept = default;  // Utilisé pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements_ < capacite_);  // Comme dans le TD précédent, on ne demande pas la réallocation pour ListeActeurs...
		elements_[nElements_++] = move(element);
	}

	// Noter que ces accesseurs const permettent de modifier les éléments; on pourrait vouloir des versions const qui retournent des const shared_ptr, et des versions non const qui retournent des shared_ptr.
	shared_ptr<T>& operator[] (int index) const { return elements_[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements_.get(), nElements_); }

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
using ListeActeurs = Liste<Acteur>;


// Interface pour l'affichage d'objets (classe abstraite pure).
class Affichable
{
public:
	virtual ostream& afficher(ostream& o) const = 0;
};

// Les Items supportent l'interface Affichable.
class Item : public Affichable
{
public:
	Item() : titre_(""), anneeSortie_(0){}
	Item(const string& titre, int anneeSortie) : titre_(titre), anneeSortie_(anneeSortie) {}

	// Accesseurs en lecture :
	const string& lireTitre() const { return titre_; }
	int lireAnneSortie() const { return anneeSortie_; }

	// Accesseurs en écriture :
	void ecrireTitre(const string& titre) { titre_ = titre; }
	void ecrireAnneeSortie(int anneeSortie) { anneeSortie_ = anneeSortie; }

	// Affichage :
	ostream& afficher(ostream& o) const override { return o << "\nTitre :\t\t\t" << titre_ << "\nDate :\t\t\t" << anneeSortie_; }

protected:
	string titre_;
	int anneeSortie_;
};

class Livre : public virtual Item
{
public:
	Livre() : auteur_(""), nCopiesVendues_(0), nPages_(0) {}
	Livre(const string& titre, int anneeSortie, const string& auteur, int nCopiesVendues, int nPages) : Item(titre, anneeSortie)
	{
		auteur_ = auteur;
		nCopiesVendues_ = nCopiesVendues;
		nPages_ = nPages;
	}

	// Accesseurs en lecture :
	const string& lireAuteur() const { return auteur_; }
	int lireNCopiesVendues() const { return nCopiesVendues_; }
	int lireNPages() const { return nPages_; }

	// Affichage :
	ostream& afficherAux(ostream& o) const; // Permet d'afficher uniquement les informations propres aux Livres (utile pour afficher des FilmLivres).
	ostream& afficher(ostream& o) const;

protected:
	string auteur_; // Nom de l'auteur (on suppose qu'il n'y a qu'un auteur).
	int nCopiesVendues_; // Nombre d'exemplaires vendus, en millions.
	int nPages_; // Nombre de pages.
};

class Film : public virtual Item
{
public:
	Film() : realisateur_(""), recette_(0), acteurs_(ListeActeurs()){}

	// Accesseurs en lecture :
	int lireRecette() const { return recette_; }
	const string& lireRealisateur() const { return realisateur_; }
	const ListeActeurs& obtenirActeurs() const { return acteurs_; }

	// Accesseurs en écriture :
	void ecrireRecette(int recette) { recette_ = recette; }
	void ecrireRealisateur(const string& realisateur) { realisateur_ = realisateur; }
	ListeActeurs& obtenirActeurs() { return acteurs_; }

	// Affichage :
	ostream& afficher(ostream& o) const;

protected:
	string realisateur_; // Nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int recette_; // Recette globale du film en millions de dollars.
	ListeActeurs acteurs_;
};

struct Acteur
{
	string nom; int anneeNaissance=0; char sexe='\0';
};

class FilmLivre : public Film, public Livre
{
public:
	FilmLivre(const Film& film, const Livre& livre) 
	{
		// On utilise le titre et l'année du film.
		titre_ = film.lireTitre();
		anneeSortie_ = film.lireAnneSortie();

		realisateur_ = film.lireRealisateur();
		recette_ = film.lireRecette();
		acteurs_ = ListeActeurs(film.obtenirActeurs());

		auteur_ = livre.lireAuteur();
		nCopiesVendues_ = livre.lireNCopiesVendues();
		nPages_ = livre.lireNPages();
	}

	// Affichage :
	ostream& afficher(ostream& o) const;
};