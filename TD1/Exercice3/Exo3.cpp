/**
* Programme qui ins�re un entier saisi au clavier dans un tableau tri�
* \file   Exo3.cpp
* \author Maya Teylouni et Julien M�tais
* \date   27 janvier 2021
* Cr�� le 27 janvier 2021
*/

#include <iostream>
#include <string>
#include <array>

using namespace std;

// La fonction qui demande � l'utilisateur de saisir un entier au clavier.
int lireEntierSaisi() {
	cout << "Saisir l'entier a inserer" << endl;

	string saisie = "";
	cin >> saisie;

	try { // On tente de convertir la cha�ne de caract�res saisie en entier.
		return stoi(saisie);
	}
	catch (const invalid_argument& exception) { //En cas d'�chec, on demande une nouvelle saisie.
		cout << "Saisie invalide" << endl;
		return lireEntierSaisi();
	}
}

// Une fonction qui affiche les �l�ments d'un tableau d'entiers dont la taille est pass�e en param�tres.
void afficherTableau(int tableau[], int taille) {
	cout << "Etat du tableau : " << endl;
	for (int k = 0; k < taille; k++) {
		cout << tableau[k] << " ";
	}
	cout << "\n" << endl;
}

const int tailleDeTableau = 6;

int main() {
	int nEntiers = tailleDeTableau - 1; // Le nombre de valeurs tri�es dans le tableau.
	int entiers[tailleDeTableau] = { 1, 5, 14, 36, 72 };  // Un tableau d'entiers tri�s avec une case libre pour insertion.

	afficherTableau(entiers, nEntiers);

	// On demande � l'utilisateur de saisir un entier au clavier.
	int entierSaisi = lireEntierSaisi();
	cout << "Saisie : " << entierSaisi << "\n" << endl;

	// On cherche la position � laquelle ins�rer l'entier saisi.
	int positionInsertion = 0;
	while (entierSaisi > entiers[positionInsertion] && positionInsertion < nEntiers) {
		positionInsertion++;
	}

	// On d�cale toutes les valeurs � droite de l'indice positionInsertion pour lib�rer un place.
	for (int j = nEntiers; j > positionInsertion; j--) {
		entiers[j] = entiers[j - 1];
	}

	// On ins�re l'entier saisi � la position trouv�e.
	entiers[positionInsertion] = entierSaisi;
	nEntiers++;

	afficherTableau(entiers, nEntiers);
	
	return 0;
}
