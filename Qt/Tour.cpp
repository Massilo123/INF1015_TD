/**
* Implémentation des méthodes de la classe Tour du modèle pour le projet de jeu d'échecs.
* \file   Tour.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   11 avril 2021
* Créé le 11 avril 2021
*/

#include "Tour.h"

using iter::range;

namespace modele
{

	bool Tour::estUneDestinationValide(const Position& arrivee) const
	{
		// La tour peut se déplacer le long d'une ligne ou d'une colonne :
		return (position_.i == arrivee.i || position_.j == arrivee.j) && position_ != arrivee;
	}

	std::unordered_set<Position, PositionHasher> Tour::obtenirChemin(const Position& arrivee) const
	{
		std::unordered_set<Position, PositionHasher> chemin = {};

		int sensDéplacementColonne = position_.i - arrivee.i > 0 ? -1 : 1;
		int sensDéplacementLigne = position_.j - arrivee.j > 0 ? -1 : 1;

		// Si la tour se déplace le long d'une colonne :
		for (int d : range(1, abs(position_.i - arrivee.i)))
		{
			chemin.insert({ position_.i + d * sensDéplacementColonne, position_.j });
		}

		// Si la tour se déplace le long d'une ligne :
		for (int d : range(1, abs(position_.j - arrivee.j)))
		{
			chemin.insert({ position_.i, position_.j + d * sensDéplacementLigne });
		}

		return chemin;
	}

}