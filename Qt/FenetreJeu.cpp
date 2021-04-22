/**
* Implémentation du vue-contrôleur pour le projet de jeu d'échecs.
* \file   FenetreJeu.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   14 avril 2021
* Créé le 14 avril 2021
*/


#include "FenetreJeu.h"

#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#pragma pop()
#include <iostream>
#include <type_traits>
#include <cppitertools/range.hpp>

using iter::range;

//template <typename T>
QPushButton* FenetreJeu::nouveauBouton(const QString& text)
{
	auto bouton = new QPushButton(this);
	bouton->setText(text);
	bouton->setFixedSize(80, 80);
	//if constexpr (!is_same_v<T, decltype(nullptr)>)
	//	QObject::connect(bouton, &QPushButton::clicked, &partie_, slot);
	return bouton;
}

FenetreJeu::FenetreJeu(QWidget* parent) :
	QMainWindow(parent)
{
	// Si on objet n'a pas encore de parent on lui met "this" comme parent en attendant, si possible, pour s'assurer que tous les pointeurs sont gérés par un delete automatique en tout temps sans utiliser de unique_ptr.
	auto widgetPrincipal = new QWidget(this);
	auto layoutPrincipal = new QVBoxLayout(widgetPrincipal);  // Donner un parent à un layout est comme un setLayout.
	layoutPrincipal->setSpacing(0);

	auto labels = new QHBoxLayout();
	layoutPrincipal->addLayout(labels);
	labels->setSpacing(0);
	std::string nomsColonnes_[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };

	for (int i : range(8)) 
	{
		auto ligne = new QHBoxLayout();
		layoutPrincipal->addLayout(ligne);
		ligne->setSpacing(0);

		auto label = new QLabel();
		std::string text = std::to_string(8-i);
		label->setText(QString(text.c_str()));
		ligne->addWidget(label);

		auto colLabel = new QLabel();
		std::string lettre = nomsColonnes_[i];
		colLabel->setText(QString(lettre.c_str()));
		colLabel->setAlignment(Qt::AlignCenter);
		labels->addWidget(colLabel);

		for (int j : range(8)) 
		{
			auto bouton = nouveauBouton(QString(""));
			
			bouton->setProperty("position", QVariant::fromValue<QPair<int, int>>(QPair<int, int>(i, j)));
			grille_[i][j] = bouton;
			ligne->addWidget(bouton);

			QObject::connect(bouton, &QPushButton::clicked, this, &FenetreJeu::selectionnerCase);
		}
	}

	QObject::connect(&partie_, &Partie::pieceDeplacee, this, &FenetreJeu::mettreAJourAffichage);
	QObject::connect(&partie_, &Partie::mouvementInvalide, this, &FenetreJeu::retroactionErreur);

	setCentralWidget(widgetPrincipal);
	setWindowTitle("Jeu d'échecs");
}

void FenetreJeu::selectionnerCase()
{
	if ((i_ + j_) % 2 == 1)
	{
		grille_[i_][j_]->setStyleSheet("background-color: black;color:white");
	}
	else
	{
		grille_[i_][j_]->setStyleSheet("background-color: white;color:black");
	}

	int i = QObject::sender()->property("position").value<QPair<int, int>>().first;
	int j = QObject::sender()->property("position").value<QPair<int, int>>().second;

	grille_[i][j]->setStyleSheet("background-color: rgb(30,70,150);color:white");
	i_ = i;
	j_ = j;

	partie_.caseSelectionnee({ i,j });
}

void FenetreJeu::mettreAJourAffichage()
{
	for (int i : range(8))
	{
		for (int j : range(8))
		{
			std::string nomPiece = partie_.lireContenuCase(i, j);
			if (nomPiece != "")
			{
				std::string fichier = "./images/" + nomPiece + ".png";
				QPixmap pixmap(fichier.c_str());
				QIcon ButtonIcon(pixmap);
				grille_[i][j]->setIcon(ButtonIcon);
				//grille_[i][j]->setIconSize(pixmap.rect().size());
				grille_[i][j]->setIconSize(QSize(70, 70));
			}
			else
			{
				grille_[i][j]->setIcon(QIcon());
			}


			if ((i + j) % 2 == 1)
			{
				grille_[i][j]->setStyleSheet("background-color: black;color:white");
			}
			else
			{
				grille_[i][j]->setStyleSheet("background-color: white;color:black");
			}
		}
	}
}

void FenetreJeu::retroactionErreur(int i, int j)
{
	grille_[i][j]->setStyleSheet("background-color: rgb(150,25,25);color:white");
}

//void FenetreJeu::chargerPartie(const Partie& partie)
//{
//	partie_ = std::make_unique<Partie>(partie);
//}


void FenetreJeu::ajouterPiece(std::unique_ptr<Piece> piece)
{
	partie_.ajouterPiece(std::move(piece));
	mettreAJourAffichage();
}
