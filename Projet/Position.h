/**
* D�claration de la strcture de Position du mod�le pour le projet de jeu d'�checs.
* \file   Partie.cpp
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   7 avril 2021
* Cr�� le 7 avril 2021
*/

#pragma once

#include <utility>

// Structure pour repr�senter la position d'une pi�ce sur l'�chiquier.
struct Position {
	int i; // Indice de la ligne (entre 0 et 7).
	int j; // Indice de la colonne (entre 0 et 7).

	bool operator==(const Position& autre) const
	{
		return (i == autre.i && j == autre.j);
	}
};

// Pour pouvoir utiliser des Positions comme cl�s dans des maps ou sets, on d�finit une fonction de hachage simple :
struct PositionHasher
{
	std::size_t operator()(const Position& pos) const
	{
		return std::hash<int>()(pos.i) ^ std::hash<int>()(pos.j);
	}
};
