/**
* Implémentation des méthodes de la classe Roi du modèle pour le projet de jeu d'échecs.
* \file   Roi.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#include "Roi.h"

bool Roi::estUneDestinationValide(const Position& arrivee) const
{
	// Le roi peut se déplacer d'une case dans n'importe quelle direction :
	return abs(position_.i - arrivee.i) < 2 && abs(position_.j - arrivee.j) < 2 && position_!=arrivee;
}

std::unordered_set<Position, PositionHasher> Roi::obtenirChemin(const Position& arrivee) const
{
	// Le roi ne traverse aucune case avant d'arriver à sa destination.

	//Remarque : le paramètre arrivee n'étant pas utilisé, un avertissement est généré.
	return {};
}