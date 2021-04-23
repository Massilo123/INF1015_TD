/**
* Déclaration de la classe Cavalier du modèle pour le projet de jeu d'échecs.
* \file   Cavalier.h
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   11 avril 2021
* Créé le 11 avril 2021
*/

#pragma once

#include "Piece.h"

namespace modele
{

	class Cavalier : public Piece {
	public:
		Cavalier(int i, int j, bool estBlanc) : Piece(i, j, estBlanc) {}

		bool estUneDestinationValide(const Position& arrivee) const override;
		std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const override;

		const std::string& lireNom() const override { return nom_; }

	private:
		std::string nom_ = "Cavalier"; // Utile pour l'affichage textuel de l'état de la partie.
	};

}