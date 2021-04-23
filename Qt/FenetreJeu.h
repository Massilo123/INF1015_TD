/**
* Vue-contr�leur pour le projet de jeu d'�checs.
* \file   FenetreJeu.h
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   14 avril 2021
* Cr�� le 14 avril 2021
*/

#pragma once

#include "Partie.h"

#pragma warning(push, 0) // Sinon Qt fait des avertissements � /W4.
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLabel>
#pragma pop()

namespace vue
{

	class FenetreJeu : public QMainWindow {
		Q_OBJECT

	public:
		FenetreJeu(QWidget* parent = nullptr);
		~FenetreJeu() override = default;

		// Utile pour tester des configurations de jeu.
		void ajouterPiece(std::unique_ptr<modele::Piece> piece);

	public slots:

		void selectionnerCase(); // R�troaction visuelle lorsque l'utilisateur clique sur une case.
		void mettreAJourAffichage(); // Affiche l'ensemble des pi�ces � leurs positions sur l'�chiquier.
		void retroactionErreur(int i, int j); // R�troaction visuelle lorsqu'un mouvement invalide est demand�.

	private:

		QPushButton* nouveauBouton(const QString& text);

		modele::Partie partie_;  // Le Mod�le (partie d'�checs).

		QPushButton* grille_[8][8]; // Les boutons constituant la vue (1 par case de l'�chiquier).

		// Position de la case s�lectionn�e.
		int i_;
		int j_;
	};

}

