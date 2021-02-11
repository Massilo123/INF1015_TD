/**
* Fichier contenant la déclaration des structures pour manipuler une collection de films.
* \file   td2.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   10 février 2021
* Créé le 2 février 2021
*/


#pragma once

#include <string>


struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms
{
private:
	int capacite; // Nombre de films pouvant être contenus dans la structure avec la mémoire allouée pour le tableau dynamique.
	int nElements; // Nombre de films contenus dans la structure.
	Film** elements; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Acteur.

public:
	ListeFilms(); // Constructeur par défaut : initialise une liste vide avec une capacité de 1.

	void lire(const std::string& nomFichier); // Remplit la liste à partir du contenu d'un fichier.
	void detruire(); // Libère la mémoire allouée pour la liste de films.

	void afficher() const; // Affiche l'ensemble des films de la liste.

	void ajouterFilm(Film* film); // Ajoute un film à la liste, alloue de la mémoire supplémentaire si nécessaire.
	void retirerFilm(Film* film); // Retire un film de la liste, ne conserve pas l'ordre au sein de la liste.

	Acteur* rechercherActeur(const std::string& nom) const; // Renvoie un pointeur vers un acteur ayant le nom indiqué, ou nullptr si cet acteur est introuvable dans la liste.

	// Accesseurs
	int obtenirNElements() const; // Renvoie le nombre de films contenus dans la liste.
	Film* obtenirFilm(int id) const; // Renvoie un pointeur vers le film de rang "id" dans la liste.
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
