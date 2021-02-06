/**
* Programme qui calcule et affiche la limite d'une suite
* \file   Exo2.cpp
* \author Maya Kurdi-Teylouni et Julien M�tais
* \date   5 f�vrier 2021
* Cr�� le 26 janvier 2021
*/

#include <iostream>
#include <iomanip>  

using namespace std;

double calculeLimite(int precision) // Fonction qui calcule la limite de la suite avec un nombre de chiffres apr�s la virgules pass� en argument.
{
	double uPresent = 1.0; // Le terme initial U0 = 1 de la suite.
	double uProchain = pow((2 + uPresent), 0.5); // Calcul du prochain terme de la suite.

	while((uProchain - uPresent) > pow(10, -precision)) // Calcul des termes de la suite tant que la diff�rence entre 2 termes cons�cutifs est sup�rieure � la pr�cision voulue.
	{
		uPresent = uProchain;
		uProchain = pow((2 + uPresent), 0.5);
	}
	return uProchain;
}

int main() //Affichage de la limite dans main
{
	int precision = 5; // 5 chiffres apr�s la virgule.
	
	cout << "La limite de cette suite est de " << setiosflags(ios::fixed) << setprecision(precision) << calculeLimite(precision) << endl;
}

