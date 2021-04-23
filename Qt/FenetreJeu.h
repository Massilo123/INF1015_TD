/**
* Vue-contrôleur pour le projet de jeu d'échecs.
* \file   FenetreJeu.h
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   14 avril 2021
* Créé le 14 avril 2021
*/

#pragma once

#include "Partie.h"

#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
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

		void selectionnerCase(); // Rétroaction visuelle lorsque l'utilisateur clique sur une case.
		void mettreAJourAffichage(); // Affiche l'ensemble des pièces à leurs positions sur l'échiquier.
		void retroactionErreur(int i, int j); // Rétroaction visuelle lorsqu'un mouvement invalide est demandé.

	private:

		QPushButton* nouveauBouton(const QString& text);

		modele::Partie partie_;  // Le Modèle (partie d'échecs).

		QPushButton* grille_[8][8]; // Les boutons constituant la vue (1 par case de l'échiquier).

		// Position de la case sélectionnée.
		int i_;
		int j_;
	};

}

