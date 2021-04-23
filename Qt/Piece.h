/**
* Déclaration de la classe Pièce du modèle pour le projet de jeu d'échecs.
* \file   Piece.h
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#pragma once

#include <cstdlib>
#include <unordered_set>
#include <string>

#include "Position.h"

namespace modele
{

	// Classe virtuelle représentant une pièce du jeu d'échecs.
	class Piece {
	public:
		Piece(int i, int j, bool estBlanc) : position_({ i, j }), estBlanc_(estBlanc) {}
		Piece(const Position& position, bool estBlanc) : position_(position), estBlanc_(estBlanc) {}

		virtual ~Piece() = default;

		// Accesseurs :
		Position lirePosition() const { return position_; }
		void ecrirePosition(const Position& position) { position_ = position; }
		virtual const std::string& lireNom() const = 0;

		bool estBlanc() const { return estBlanc_; }

		// Indique si le déplacement de la pièce peut lui permettre de se rendre sur la position visée (sans tenir compte des autres pièces).
		virtual bool estUneDestinationValide(const Position& arrivee) const = 0;

		// Indique les positions que la pièce devrait traverser pour atteindre la destination (excluant les cases de départ et d'arrivée).
		virtual std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const = 0;

	protected:
		Position position_; // Position de la pièce sur l'échiquier.
		bool estBlanc_; // Indique le camp auquel appartient la pièce.
	};

}