/**
* Implémentation des opérations sur le modèle pour le projet de jeu d'échecs.
* \file   Partie.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

#include "Partie.h"

using iter::range;

namespace modele
{

	// Ajoute une pièce sur l'échiquier.
	void Partie::ajouterPiece(std::unique_ptr<Piece> piece)
	{
		Position pos = piece->lirePosition();

		std::cout << "\nCréation d'un.e " << piece->lireNom() << (piece->estBlanc() ? " blanc.he " : " noir.e ") << "en " << nomsColonnes_[pos.j] << nomsLignes_[pos.i] << std::endl;

		echiquier_.insert({ pos, std::move(piece) });
	}

	// Vérifie si le roi du camp passé en argument est en échec.
	bool Partie::estEnEchec(bool estBlanc) const
	{
		bool estEnEchec = false;

		// On commence par chercher la position du roi considéré.
		bool roiTrouvé = false;
		Position positionRoi = { -1, -1 };
		for (auto&& it : echiquier_)
		{
			if (dynamic_cast<Roi*>(it.second.get()) != nullptr && it.second->estBlanc() == estBlanc)
			{
				roiTrouvé = true;
				positionRoi = it.second->lirePosition();
				break;
			}
		}

		// Puis on vérifie pour chaque pièce si elle peut prendre ce roi.
		if (roiTrouvé)
		{
			for (auto&& it : echiquier_)
			{
				if (estUnMouvementValide(it.first, positionRoi) && it.second->estBlanc() != estBlanc)
				{
					estEnEchec = true;
					break;
				}
			}
		}
		return estEnEchec;
	}

	// Vérifie si le déplacement de la pièce se trouvant sur la case de départ jusqu'à la case d'arrivée est licite.
	bool Partie::estUnMouvementValide(const Position& depart, const Position& arrivee) const
	{
		bool estUnMouvementValide = true;

		if (echiquier_.at(depart)->estUneDestinationValide(arrivee)) // Si le type de pièce peut permettre le déplacement.
		{
			std::unordered_set<Position, PositionHasher> chemin = echiquier_.at(depart)->obtenirChemin(arrivee);

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
			if (echiquier_.find(arrivee) != echiquier_.end() && echiquier_.at(depart)->estBlanc() == echiquier_.at(arrivee)->estBlanc())
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
		// On vérifie qu'un pièce a bien été sélectionnée.
		if (echiquier_.find(depart) != echiquier_.end())
		{
			// On vérifie que la pièce sélectionnée appartient au bon camp.
			if (echiquier_[depart]->estBlanc() == estTourBlanc)
			{
				// On vérifie que le mouvement demandé est licite.
				if (estUnMouvementValide(depart, arrivee))
				{
					// Si la case d'arrivée contient une pièce, on la stocke.
					bool piecePrise = echiquier_.find(arrivee) != echiquier_.end();
					std::unique_ptr<Piece> detruite = nullptr;
					if (piecePrise)
					{
						detruite = move(echiquier_[arrivee]);
					}

					// On commence par simuler le mouvement demandé.
					echiquier_[arrivee] = move(echiquier_[depart]);
					echiquier_.erase(depart);
					echiquier_[arrivee]->ecrirePosition(arrivee);

					// Si on aboutit à une situation d'échec pour le camp qui vient de jouer, on annule le mouvement.
					if (estEnEchec(estTourBlanc))
					{
						std::cout << "\nMouvement impossible (échec) !" << std::endl;

						echiquier_[depart] = move(echiquier_[arrivee]);
						echiquier_.erase(arrivee);
						echiquier_[depart]->ecrirePosition(depart);
						if (piecePrise)
						{
							echiquier_[arrivee] = move(detruite);
						}
						afficher();

						emit mouvementInvalide(arrivee.i, arrivee.j); // On informe la vue d'une erreur sur le mouvement demandé pour rétroaction visuelle.
					}
					else
					{
						// Si le mouvement est validé, on passe au tour suivant.
						estTourBlanc = !estTourBlanc;
						emit pieceDeplacee();

						std::cout << "\nDéplacement du " << echiquier_[arrivee]->lireNom() << (echiquier_[arrivee]->estBlanc() ? " blanc.he " : " noir.e ") << "vers " << nomsColonnes_[arrivee.j] << nomsLignes_[arrivee.i] << std::endl;
					}
				}
				else
				{
					std::cout << "\nMouvement invalide !" << std::endl;
					emit mouvementInvalide(arrivee.i, arrivee.j);
				}
			}
			else
			{
				std::cout << "\nCe n'est pas votre tour !" << std::endl;
				emit mouvementInvalide(arrivee.i, arrivee.j);
			}
		}
		else
		{
			std::cout << "\nAucune pièce sélectionnée !" << std::endl;
			emit mouvementInvalide(arrivee.i, arrivee.j);
		}
	}


	// Affichage textuel de l'état de la partie.
	void Partie::afficher() const
	{
		std::cout << "\nTOUR DES " << (estTourBlanc ? "BLANCS" : "NOIRS") << std::endl;
		for (auto&& it : echiquier_)
		{
			std::cout << it.second->lireNom() << (it.second->estBlanc() ? " blanc.he " : " noir.e ") << " " << nomsColonnes_[it.first.j] << nomsLignes_[it.first.i] << std::endl;
		}
	}

	std::string Partie::lireContenuCase(int i, int j) const
	{
		Position position = { i, j };
		std::string nomPiece = "";

		if (echiquier_.find(position) != echiquier_.end())
		{
			nomPiece = echiquier_.at(position)->lireNom() + (echiquier_.at(position)->estBlanc() ? "blanc" : "noir");
		}
		return nomPiece;
	}


	// Définit le comportement lorsqu'on clique sur une case.
	void Partie::caseSelectionnee(const Position& selection)
	{
		// S'il n'y avait aucune sélection en mémoire, on stocke celle-ci.
		if (selection_ == nullptr)
		{
			selection_ = new Position(selection);
		}
		// Sinon, on utilise la nouvelle sélection comme destination et l'ancienne comme départ pour effectuer un mouvement.
		else
		{
			deplacerPiece(*selection_, selection);
			delete(selection_); // On efface la sélection en mémoire.
			selection_ = nullptr;
		}
	}

}