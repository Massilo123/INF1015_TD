/**
* Programme qui insère un entier saisi au clavier dans un tableau trié
* \file   Exo3.cpp
* \author Maya Teylouni et Julien Métais
* \date   27 janvier 2021
* Créé le 27 janvier 2021
*/

#include <iostream>
#include <string>
#include <array>

using namespace std;

// La fonction qui demande à l'utilisateur de saisir un entier au clavier.
int lireEntierSaisi() {
	cout << "Saisir l'entier a inserer" << endl;

	string saisie = "";
	cin >> saisie;

	try { // On tente de convertir la chaîne de caractères saisie en entier.
		return stoi(saisie);
	}
	catch (const invalid_argument& exception) { //En cas d'échec, on demande une nouvelle saisie.
		cout << "Saisie invalide" << endl;
		return lireEntierSaisi();
	}
}

// Une fonction qui affiche les éléments d'un tableau d'entiers dont la taille est passée en paramètres.
void afficherTableau(int tableau[], int taille) {
	cout << "Etat du tableau : " << endl;
	for (int k = 0; k < taille; k++) {
		cout << tableau[k] << " ";
	}
	cout << "\n" << endl;
}

const int tailleDeTableau = 6;

int main() {
	int nEntiers = tailleDeTableau - 1; // Le nombre de valeurs triées dans le tableau.
	int entiers[tailleDeTableau] = { 1, 5, 14, 36, 72 };  // Un tableau d'entiers triés avec une case libre pour insertion.

	afficherTableau(entiers, nEntiers);

	// On demande à l'utilisateur de saisir un entier au clavier.
	int entierSaisi = lireEntierSaisi();
	cout << "Saisie : " << entierSaisi << "\n" << endl;

	// On cherche la position à laquelle insérer l'entier saisi.
	int positionInsertion = 0;
	while (entierSaisi > entiers[positionInsertion] && positionInsertion < nEntiers) {
		positionInsertion++;
	}

	// On décale toutes les valeurs à droite de l'indice positionInsertion pour libérer un place.
	for (int j = nEntiers; j > positionInsertion; j--) {
		entiers[j] = entiers[j - 1];
	}

	// On insère l'entier saisi à la position trouvée.
	entiers[positionInsertion] = entierSaisi;
	nEntiers++;

	afficherTableau(entiers, nEntiers);
	
	return 0;
}
