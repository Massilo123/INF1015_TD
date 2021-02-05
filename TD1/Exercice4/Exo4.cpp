/**
* Programme qui calcule des produits d'entiers par méthode russe
* \file   Exo4.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   26 janvier 2021
* Créé le 26 janvier 2021
*/

#include <vector>
#include <numeric>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Fonction qui calcule par méthode russe et renvoie le produit de 2 entiers passés en paramètres.
int calculeProduit(int a, int b) {
	int gauche = a; // Représente le terme courant dans la colonne de gauche du tableau.
	int droite = b; // Terme courant de la colonne de droite.

	vector<int> termesASommer = {}; // Stocke tous les termes non rayés dans la colonne de droite.

	while (gauche >= 1) {
		if (gauche % 2 != 0) { // Si le terme de gauche est impair, on stocke le terme de droite.
			termesASommer.push_back(droite);
		}

		gauche = gauche / 2;
		droite = droite * 2;
	}

	return accumulate(termesASommer.begin(), termesASommer.end(), 0); // On renvoie la somme de tous les termes non rayés dans la colonne de droite.
}

// Le type utilisé pour représenter une paire de valeurs sur laquelle tester notre fonction.
struct TestDeProduit {
	int facteur1;
	int facteur2;
	int resultatAttendu;
};

const int nTests = 3; // Le nombre de tests à effectuer.

int main() {

	srand(time(NULL)); // Initialisation du générateur de nombres aléatoires.

	TestDeProduit tests[nTests] = {}; // Le tableau contenant nos tests.

	int valeurMaximale = 200;
	for (int i = 0; i < nTests; i++) { // On remplit le tableau en générant aléatoirement des paires d'entiers entre 0 et valeurMaximale.
		int a = rand() % valeurMaximale;
		int b = rand() % valeurMaximale;
		int r = a * b;

		tests[i] = { a, b, r };
	}

	int i = 0;
	bool testRéussi = true;
	while (i < nTests && testRéussi) { // On s'assure que tous les tests sont réussis.
		testRéussi = (calculeProduit(tests[i].facteur1, tests[i].facteur2) == tests[i].resultatAttendu);
		i += 1;
	}

	if (testRéussi) {
		cout << nTests << "/" << nTests << " tests passent" << endl;
	}

	return 0;
}