/**
* Implémentation des méthodes de la classe Cavalier du modèle pour le projet de jeu d'échecs.
* \file   Cavalier.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   11 avril 2021
* Créé le 11 avril 2021
*/

#include "Cavalier.h"

using iter::range;

namespace modele
{

	bool Cavalier::estUneDestinationValide(const Position& arrivee) const
	{
		// Le cavalier peut se déplacer en L :
		return (abs(position_.i - arrivee.i) == 2 && abs(position_.j - arrivee.j) == 1) || (abs(position_.i - arrivee.i) == 1 && abs(position_.j - arrivee.j) == 2);
	}

	std::unordered_set<Position, PositionHasher> Cavalier::obtenirChemin(const Position& arrivee) const
	{
		// Le cavalier ne traverse aucune case pour arriver à sa destination.

		//Remarque : le paramètre arrivee n'étant pas utilisé, un avertissement est généré.
		return {};
	}

}