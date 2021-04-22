/**
* D�claration de la classe Roi du mod�le pour le projet de jeu d'�checs.
* \file   Roi.h
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   7 avril 2021
* Cr�� le 7 avril 2021
*/

#pragma once

#include "Piece.h"

class Roi : public Piece {
public :
	Roi(int i, int j, bool estBlanc) : Piece(i, j, estBlanc) {}

	bool estUneDestinationValide(const Position& arrivee) const override;
	std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const override;

	const std::string& lireNom() const override { return nom_; }

private:
	std::string nom_ = "Roi"; // Utile pour l'affichage textuel de l'�tat de la partie.
};
