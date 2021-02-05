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

double limite(double precision) //fonction qui calcule la limite de la suite
{

	double uPresent = 1.0;//U0=1
	
	double uProchain = pow((2 + uPresent), 0.5);//Calcul du prochain terme de la suite
	while ((uProchain - uPresent) > 0.00001)/*Calcul du prochain terme tant que
		la diff�rence entre le terme prochain 
		et le terme pr�sent est sup�rieure � la pr�cision de la limite*/
	{
		uPresent = uProchain;
		uProchain = pow((2 + uPresent), 0.5);
	

			if ((uProchain - uPresent) <= 0.00001) /*quand la diff�rence entre terme 
				prochain et terme pr�sent est inf�rieure ou �gale � la pr�cision 
				de la limite, on retourne le dernier uprochain calcul� comme limite*/
			{
			
				uProchain = (double)(uProchain / precision) * precision;
				return uProchain;
			}
			

	}
	
}

int main() //Affichage de la limite dans main
{
	
	double precision = 5.0;//5 chiffres apr�s la virgule
	double uProchain = limite(precision);
	
	cout << "La limite de cette suite est de " << setiosflags(ios::fixed) << setprecision(precision) << limite(precision);

}

