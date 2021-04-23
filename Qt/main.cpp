/**
* Fichier principal du projet de jeu d'échecs, permettant l'exécution d'une partie ainsi que les tests du programme.
* \file   main.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   7 avril 2021
* Créé le 7 avril 2021
*/

//#include "Calc.hpp"
//#include "CalcWindow.hpp"
#include "FenetreJeu.h"

#include <QApplication>

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	// cdbg.setTee(&clog);  // Décommenter cette ligne pour que cdbg affiche sur la console en plus de la "Sortie" du débogueur.

	bibliotheque_cours::executerGoogleTest(argc, argv); // Attention de ne rien afficher avant cette ligne, sinon l'Explorateur de tests va tenter de lire votre affichage comme un résultat de test.
#endif
//NOTE: C'est normal que la couverture de code dans l'Explorateur de tests de Visual Studio ne couvre pas la fin de cette fonction ni la fin du main après l'appel à cette fonction puisqu'il exécute uniquement les tests Google Test dans l'appel ci-dessus.
}

int main(int argc, char* argv[])
{
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	QApplication app(argc, argv);
	initialiserBibliothequeCours(argc, argv);

	//CalcWindow calcWindow;
	//calcWindow.show();
	

	// Série de tests préliminaires pour vérifier qu’on peut bien bouger les pièces seulement pour des mouvements valides
	/*
	//Début de la partie
	Partie partie;
	partie.afficher();

	// Création d'un roi blanc en C5
	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	// Déplacement du roi blanc vers D6
	partie.deplacerPiece({ 4, 2 }, { 5, 3 });
	partie.afficher();

	// Tentative de mouvement depuis la case C5 laissée vide
	partie.deplacerPiece({ 4, 2 }, { 6, 3 });
	partie.afficher();

	// Tentative de déplacement du roi blanc de plus d'une case
	partie.deplacerPiece({ 5, 3 }, { 6, 5 });
	partie.afficher();

	// Création d'un roi blanc en C5
	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	// Tentative de prise du second roi blanc par le premier
	partie.deplacerPiece({ 5, 3 }, { 4, 2 });
	partie.afficher();

	// Création d'une tour noire en D1
	partie.ajouterPiece(std::make_unique<Tour>(0, 3, false));
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	// Déplacement de la tour noire vers D5
	partie.deplacerPiece({ 0, 3 }, { 4, 3 });
	partie.afficher();

	// Tentative de déplacement de la tour noire par dessus le roi blanc se trouvant en D6
	partie.deplacerPiece({ 4, 3 }, { 6, 3 });
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	// Prise du roi blanc se trouvant en D6 par la tour noire
	partie.deplacerPiece({ 4, 3 }, { 5, 3 });
	partie.afficher();

	// Création d'un cavalier blanc en E4
	partie.ajouterPiece(std::make_unique<Cavalier>(3, 4, true));
	partie.afficher();

	// Tentative de déplacement du cavalier le long d'une ligne
	partie.deplacerPiece({ 3, 4 }, { 3, 7 });
	partie.afficher();

	// Prise de la tour noire par le cavalier blanc
	partie.deplacerPiece({ 3, 4 }, { 5, 3 });
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	// Déplacement du cavalier blanc de D6 vers B5
	partie.deplacerPiece({ 5, 3 }, { 4, 1 });
	partie.afficher();
	*/
	vue::FenetreJeu fenetre;
	fenetre.ajouterPiece(std::make_unique<modele::Roi>(4, 2, true));
	fenetre.ajouterPiece(std::make_unique<modele::Tour>(0, 3, false));
	fenetre.ajouterPiece(std::make_unique<modele::Cavalier>(3, 4, true));
	fenetre.ajouterPiece(std::make_unique<modele::Roi>(1, 6, false));
	fenetre.ajouterPiece(std::make_unique<modele::Tour>(4, 1, true));
	fenetre.ajouterPiece(std::make_unique<modele::Cavalier>(7, 0, false));

	try
	{
		fenetre.ajouterPiece(std::make_unique<modele::Roi>(2, 6, false));
	}
	catch(std::logic_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	fenetre.show();

	return app.exec();
}
