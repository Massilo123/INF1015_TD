/**
* Programme qui calcule et affiche la limite d'une suite
* \file   Exo2.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   5 février 2021
* Créé le 26 janvier 2021
*/

#include <iostream>
#include <iomanip>  

using namespace std;

double calculeLimite(int precision) // Fonction qui calcule la limite de la suite avec un nombre de chiffres après la virgules passé en argument.
{
	double uPresent = 1.0; // Le terme initial U0 = 1 de la suite.
	double uProchain = pow((2 + uPresent), 0.5); // Calcul du prochain terme de la suite.

	while((uProchain - uPresent) > pow(10, -precision)) // Calcul des termes de la suite tant que la différence entre 2 termes consécutifs est supérieure à la précision voulue.
	{
		uPresent = uProchain;
		uProchain = pow((2 + uPresent), 0.5);
	}
	return uProchain;
}

int main() //Affichage de la limite dans main
{
	int precision = 5; // 5 chiffres après la virgule.
	
	cout << "La limite de cette suite est de " << setiosflags(ios::fixed) << setprecision(precision) << calculeLimite(precision) << endl;
}

