/**
* Implémentation des opérations sur le modèle pour le projet de jeu d'échecs.
* \file   Partie.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#include <cppitertools/range.hpp>

#include "Partie.h"

using iter::range;

// Ajoute une pièce sur l'échiquier.
void Partie::ajouterPiece(std::unique_ptr<Piece> piece)
{
	Position pos = piece->lirePosition();

	std::cout << "\nCréation d'un.e " << piece->lireNom() << (piece->estBlanc() ? " blanc.he " : " noir.e ") << "en " << nomsColonnes_[pos.j]  << nomsLignes_[pos.i] << std::endl;

	echiquier_.insert({ pos, std::move(piece) });
}

// Vérifie si le déplacement de la pièce se trouvant sur la case de départ jusqu'à la case d'arrivée est licite.
bool Partie::estUnMouvementValide(const Position& depart, const Position& arrivee)
{
	bool estUnMouvementValide = true;
	
	if (echiquier_[depart]->estUneDestinationValide(arrivee)) // Si le type de pièce peut permettre le déplacement.
	{
		std::unordered_set<Position, PositionHasher> chemin = echiquier_[depart]->obtenirChemin(arrivee);

		// On vérifie que toutes les cases à traverser sont libres.
		for (auto&& position : chemin)
		{
			if (echiquier_.find(position) != echiquier_.end())
			{
				estUnMouvementValide = false;
				break;
			}
		}

		// Si la case de destination est occupée par une pièce du même camp, le mouvement est invalide.
		if (echiquier_.find(arrivee) != echiquier_.end() && echiquier_[arrivee]->estBlanc() == echiquier_[arrivee]->estBlanc())
		{
			estUnMouvementValide = false;
		}
	}
	else
	{
		estUnMouvementValide = false;
	}
	
	return estUnMouvementValide;
}

// Joue un coup si le déplacement demandé est licite.
void Partie::deplacerPiece(const Position& depart, const Position& arrivee)
{
	if (echiquier_.find(depart) != echiquier_.end())
	{
		if (estUnMouvementValide(depart, arrivee))
		{
			std::cout << "\nDéplacement du " << echiquier_[depart]->lireNom() << (echiquier_[depart]->estBlanc() ? " blanc.he " : " noir.e ") << "vers " << nomsColonnes_[arrivee.j] << nomsLignes_[arrivee.i] << std::endl;

			echiquier_[arrivee] = move(echiquier_[depart]);
			echiquier_[arrivee]->ecrirePosition(arrivee);
			echiquier_.erase(depart);
		}
		else
		{
			std::cout << "\nMouvement invalide !" << std::endl;
		}
	}
	else
	{
		std::cout << "\nAucune pièce sélectionnée !" << std::endl;
	}
}


void Partie::afficher() const
{
	std::cout << "\nEtat de la partie : " << std::endl;
	for (auto&& it : echiquier_)
	{
		std::cout << it.second->lireNom() << (it.second->estBlanc() ? " blanc.he " : " noir.e ") << " " << nomsColonnes_[it.first.j] << nomsLignes_[it.first.i] << std::endl;
	}
}