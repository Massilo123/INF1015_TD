/**
* Impl�mentation des op�rations sur le mod�le pour le projet de jeu d'�checs.
* \file   Partie.cpp
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   7 avril 2021
* Cr�� le 7 avril 2021
*/

#include "Partie.h"

using iter::range;

namespace modele
{

	// Ajoute une pi�ce sur l'�chiquier.
	void Partie::ajouterPiece(std::unique_ptr<Piece> piece)
	{
		Position pos = piece->lirePosition();

		std::cout << "\nCr�ation d'un.e " << piece->lireNom() << (piece->estBlanc() ? " blanc.he " : " noir.e ") << "en " << nomsColonnes_[pos.j] << nomsLignes_[pos.i] << std::endl;

		echiquier_.insert({ pos, std::move(piece) });
	}

	// V�rifie si le roi du camp pass� en argument est en �chec.
	bool Partie::estEnEchec(bool estBlanc) const
	{
		bool estEnEchec = false;

		// On commence par chercher la position du roi consid�r�.
		bool roiTrouv� = false;
		Position positionRoi = { -1, -1 };
		for (auto&& it : echiquier_)
		{
			if (dynamic_cast<Roi*>(it.second.get()) != nullptr && it.second->estBlanc() == estBlanc)
			{
				roiTrouv� = true;
				positionRoi = it.second->lirePosition();
				break;
			}
		}

		// Puis on v�rifie pour chaque pi�ce si elle peut prendre ce roi.
		if (roiTrouv�)
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

	// V�rifie si le d�placement de la pi�ce se trouvant sur la case de d�part jusqu'� la case d'arriv�e est licite.
	bool Partie::estUnMouvementValide(const Position& depart, const Position& arrivee) const
	{
		bool estUnMouvementValide = true;

		if (echiquier_.at(depart)->estUneDestinationValide(arrivee)) // Si le type de pi�ce peut permettre le d�placement.
		{
			std::unordered_set<Position, PositionHasher> chemin = echiquier_.at(depart)->obtenirChemin(arrivee);

			// On v�rifie que toutes les cases � traverser sont libres.
			for (auto&& position : chemin)
			{
				if (echiquier_.find(position) != echiquier_.end())
				{
					estUnMouvementValide = false;
					break;
				}
			}

			// Si la case de destination est occup�e par une pi�ce du m�me camp, le mouvement est invalide.
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

	// Joue un coup si le d�placement demand� est licite.
	void Partie::deplacerPiece(const Position& depart, const Position& arrivee)
	{
		// On v�rifie qu'un pi�ce a bien �t� s�lectionn�e.
		if (echiquier_.find(depart) != echiquier_.end())
		{
			// On v�rifie que la pi�ce s�lectionn�e appartient au bon camp.
			if (echiquier_[depart]->estBlanc() == estTourBlanc)
			{
				// On v�rifie que le mouvement demand� est licite.
				if (estUnMouvementValide(depart, arrivee))
				{
					// Si la case d'arriv�e contient une pi�ce, on la stocke.
					bool piecePrise = echiquier_.find(arrivee) != echiquier_.end();
					std::unique_ptr<Piece> detruite = nullptr;
					if (piecePrise)
					{
						detruite = move(echiquier_[arrivee]);
					}

					// On commence par simuler le mouvement demand�.
					echiquier_[arrivee] = move(echiquier_[depart]);
					echiquier_.erase(depart);
					echiquier_[arrivee]->ecrirePosition(arrivee);

					// Si on aboutit � une situation d'�chec pour le camp qui vient de jouer, on annule le mouvement.
					if (estEnEchec(estTourBlanc))
					{
						std::cout << "\nMouvement impossible (�chec) !" << std::endl;

						echiquier_[depart] = move(echiquier_[arrivee]);
						echiquier_.erase(arrivee);
						echiquier_[depart]->ecrirePosition(depart);
						if (piecePrise)
						{
							echiquier_[arrivee] = move(detruite);
						}
						afficher();

						emit mouvementInvalide(arrivee.i, arrivee.j); // On informe la vue d'une erreur sur le mouvement demand� pour r�troaction visuelle.
					}
					else
					{
						// Si le mouvement est valid�, on passe au tour suivant.
						estTourBlanc = !estTourBlanc;
						emit pieceDeplacee();

						std::cout << "\nD�placement du " << echiquier_[arrivee]->lireNom() << (echiquier_[arrivee]->estBlanc() ? " blanc.he " : " noir.e ") << "vers " << nomsColonnes_[arrivee.j] << nomsLignes_[arrivee.i] << std::endl;
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
			std::cout << "\nAucune pi�ce s�lectionn�e !" << std::endl;
			emit mouvementInvalide(arrivee.i, arrivee.j);
		}
	}


	// Affichage textuel de l'�tat de la partie.
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


	// D�finit le comportement lorsqu'on clique sur une case.
	void Partie::caseSelectionnee(const Position& selection)
	{
		// S'il n'y avait aucune s�lection en m�moire, on stocke celle-ci.
		if (selection_ == nullptr)
		{
			selection_ = new Position(selection);
		}
		// Sinon, on utilise la nouvelle s�lection comme destination et l'ancienne comme d�part pour effectuer un mouvement.
		else
		{
			deplacerPiece(*selection_, selection);
			delete(selection_); // On efface la s�lection en m�moire.
			selection_ = nullptr;
		}
	}

}