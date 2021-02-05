/**
* Programme qui calcule des produits d'entiers par m�thode russe
* \file   Exo4.cpp
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   26 janvier 2021
* Cr�� le 26 janvier 2021
*/

#include <vector>
#include <numeric>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Fonction qui calcule par m�thode russe et renvoie le produit de 2 entiers pass�s en param�tres.
int calculeProduit(int a, int b) {
	int gauche = a; // Repr�sente le terme courant dans la colonne de gauche du tableau.
	int droite = b; // Terme courant de la colonne de droite.

	vector<int> termesASommer = {}; // Stocke tous les termes non ray�s dans la colonne de droite.

	while (gauche >= 1) {
		if (gauche % 2 != 0) { // Si le terme de gauche est impair, on stocke le terme de droite.
			termesASommer.push_back(droite);
		}

		gauche = gauche / 2;
		droite = droite * 2;
	}

	return accumulate(termesASommer.begin(), termesASommer.end(), 0); // On renvoie la somme de tous les termes non ray�s dans la colonne de droite.
}

// Le type utilis� pour repr�senter une paire de valeurs sur laquelle tester notre fonction.
struct TestDeProduit {
	int facteur1;
	int facteur2;
	int resultatAttendu;
};

const int nTests = 3; // Le nombre de tests � effectuer.

int main() {

	srand(time(NULL)); // Initialisation du g�n�rateur de nombres al�atoires.

	TestDeProduit tests[nTests] = {}; // Le tableau contenant nos tests.

	int valeurMaximale = 200;
	for (int i = 0; i < nTests; i++) { // On remplit le tableau en g�n�rant al�atoirement des paires d'entiers entre 0 et valeurMaximale.
		int a = rand() % valeurMaximale;
		int b = rand() % valeurMaximale;
		int r = a * b;

		tests[i] = { a, b, r };
	}

	int i = 0;
	bool testR�ussi = true;
	while (i < nTests && testR�ussi) { // On s'assure que tous les tests sont r�ussis.
		testR�ussi = (calculeProduit(tests[i].facteur1, tests[i].facteur2) == tests[i].resultatAttendu);
		i += 1;
	}

	if (testR�ussi) {
		cout << nTests << "/" << nTests << " tests passent" << endl;
	}

	return 0;
}