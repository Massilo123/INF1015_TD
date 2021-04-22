/**
* Classe principale du mod�le pour le projet de jeu d'�checs.
* \file   Partie.h
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   7 avril 2021
* Cr�� le 7 avril 2021
*/

#pragma once

#pragma warning(push, 0) // Sinon Qt fait des avertissements � /W4.
#include <QObject>
#pragma pop()
#include <functional>

#include <unordered_map>
#include <memory>

#include "Roi.h"
#include "Tour.h"
#include "Cavalier.h"

// Classe repr�sentant l'�tat de la partie.
class Partie : public QObject {
	Q_OBJECT
public:
	Partie(){};

	// Modifications de l'�tat de la partie.
	void ajouterPiece(std::unique_ptr<Piece> piece);
	void deplacerPiece(const Position& depart, const Position& arrivee);

	// V�rifications sur la configuration de la partie.
	bool estEnEchec(bool estBlanc) const;
	bool estUnMouvementValide(const Position& depart, const Position& arrivee) const;

	// Affichage textuel de l'�tat de la partie.
	void afficher() const;
	std::string lireContenuCase(int i, int j) const;

public slots:

	// D�finit le comportement lorsqu'on clique sur une case.
	void caseSelectionnee(const Position& selection);

signals:

	void pieceDeplacee(); // Informe la vue d'un d�placement pour mise � jour de l'affichage.
	void mouvementInvalide(int i, int j); // Informe la vue d'une erreur sur le mouvement demand� pour r�troaction visuelle.

private :
	// Le plateau de jeu est repr�sent� par une map o� les cl�s sont les positions et les pi�ces les valeurs.
	std::unordered_map<Position, std::unique_ptr<Piece>, PositionHasher> echiquier_;

	// Indique si c'est au tour des blancs de jouer.
	bool estTourBlanc = true; 

	// Stocke la position de d�part s�lectionn�e par l'utilisateur.
	Position* selection_ = nullptr;

	// Utile pour l'affichage textuel de l'�tat de la partie.
	std::string nomsColonnes_[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	std::string nomsLignes_[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
};