#pragma once
// Structures mémoires pour une collection de films.

#include <string>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeDeFilms
{
private:
	int capacite, nElements;
	Film** elements;

public:
	ListeDeFilms();

	void lire(const std::string& nomFichier);
	void detruire();

	void afficher() const;

	void ajouterFilm(Film* film);
	void retirerFilm(Film* film);

	Acteur* rechercherActeur(const std::string& nom) const;

	Film* getFilm(int id) const;
};


struct ListeFilms {
	int capacite, nElements;
	Film** elements; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

struct ListeActeurs {
	int capacite, nElements;
	Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
