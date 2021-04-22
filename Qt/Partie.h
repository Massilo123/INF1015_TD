/**
* Classe principale du modèle pour le projet de jeu d'échecs.
* \file   Partie.h
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#pragma once

#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>

#include <unordered_map>
#include <memory>

#include "Roi.h"
#include "Tour.h"
#include "Cavalier.h"

// Classe représentant l'état de la partie.
class Partie : public QObject {
	Q_OBJECT
public:
	Partie(){};

	// Modifications de l'état de la partie.
	void ajouterPiece(std::unique_ptr<Piece> piece);
	void deplacerPiece(const Position& depart, const Position& arrivee);

	// Vérifications sur la configuration de la partie.
	bool estEnEchec(bool estBlanc) const;
	bool estUnMouvementValide(const Position& depart, const Position& arrivee) const;

	// Affichage textuel de l'état de la partie.
	void afficher() const;
	std::string lireContenuCase(int i, int j) const;

public slots:

	// Définit le comportement lorsqu'on clique sur une case.
	void caseSelectionnee(const Position& selection);

signals:

	void pieceDeplacee(); // Informe la vue d'un déplacement pour mise à jour de l'affichage.
	void mouvementInvalide(int i, int j); // Informe la vue d'une erreur sur le mouvement demandé pour rétroaction visuelle.

private :
	// Le plateau de jeu est représenté par une map où les clés sont les positions et les pièces les valeurs.
	std::unordered_map<Position, std::unique_ptr<Piece>, PositionHasher> echiquier_;

	// Indique si c'est au tour des blancs de jouer.
	bool estTourBlanc = true; 

	// Stocke la position de départ sélectionnée par l'utilisateur.
	Position* selection_ = nullptr;

	// Utile pour l'affichage textuel de l'état de la partie.
	std::string nomsColonnes_[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	std::string nomsLignes_[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
};