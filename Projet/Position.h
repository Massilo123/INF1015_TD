/**
* Déclaration de la strcture de Position du modèle pour le projet de jeu d'échecs.
* \file   Partie.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#pragma once

#include <utility>

// Structure pour représenter la position d'une pièce sur l'échiquier.
struct Position {
	int i; // Indice de la ligne (entre 0 et 7).
	int j; // Indice de la colonne (entre 0 et 7).

	bool operator==(const Position& autre) const
	{
		return (i == autre.i && j == autre.j);
	}
};

// Pour pouvoir utiliser des Positions comme clés dans des maps ou sets, on définit une fonction de hachage simple :
struct PositionHasher
{
	std::size_t operator()(const Position& pos) const
	{
		return std::hash<int>()(pos.i) ^ std::hash<int>()(pos.j);
	}
};
