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

double limite(double precision) //fonction qui calcule la limite de la suite
{

	double uPresent = 1.0;//U0=1
	
	double uProchain = pow((2 + uPresent), 0.5);//Calcul du prochain terme de la suite
	while ((uProchain - uPresent) > 0.00001)/*Calcul du prochain terme tant que
		la différence entre le terme prochain 
		et le terme présent est supérieure à la précision de la limite*/
	{
		uPresent = uProchain;
		uProchain = pow((2 + uPresent), 0.5);
	

			if ((uProchain - uPresent) <= 0.00001) /*quand la différence entre terme 
				prochain et terme présent est inférieure ou égale à la précision 
				de la limite, on retourne le dernier uprochain calculé comme limite*/
			{
			
				uProchain = (double)(uProchain / precision) * precision;
				return uProchain;
			}
			

	}
	
}

int main() //Affichage de la limite dans main
{
	
	double precision = 5.0;//5 chiffres après la virgule
	double uProchain = limite(precision);
	
	cout << "La limite de cette suite est de " << setiosflags(ios::fixed) << setprecision(precision) << limite(precision);

}

