/**
* Programme qui affiche le mot le plus court, le mot le plus long et la longueur moyenne d'une phrase
* \file   Exo1.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   5 février 2021
* Créé le 26 janvier 2021
*/


#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include <tuple>

using namespace std;

int main()
{
    string phrase;
    std::cout << "Ecrivez une phrase svp";//L'utilisateur écrit une phrase
    getline(cin, phrase);
    
    
    string motCourt = phrase;
    string motLong = "";
    string motCourant = "";
    string motCourant1 = "";
    double nMots = 0.0;
    double longueurMoy = 0.0;

    //on passe chaque caractère de la phrase
    for (int i = 0; i < phrase.length(); i++)
    {
        int result = isspace(phrase[i]);
        if (result==0)//si le caractère actuel est un espace
        {
            motCourant += phrase[i];
        }
        if (result==8|| i == phrase.length()-1)/*si le caractère actuel n'est 
            pas un espace ou si on est au dernier caractère de la phrase*/
        {

            if (motCourant.length() < motCourt.length())/*Mise à jour de la valeur 
                du mot le plus court de la phrase*/
            {
                motCourt = motCourant;
            }
            if (motCourant.length() > motLong.length())/*Mise à jour de la valeur 
                du mot le plus long de la phrase*/
            {
                motLong = motCourant;
            }
            motCourant1 += motCourant;
            nMots += 1;
            motCourant = "";
                if (nMots != 0) //calcul de la longueur moyenne des mots dans la phrase
                    {
                    longueurMoy = motCourant1.length() / nMots;
                    }
            

        }
    }
    //Affichage du mot court, mot long et de la longueur moyenne
    cout << "Le mot le plus court est : " << motCourt << "\n";

    cout << "Le mot le plus long est :" << motLong << "\n";

    cout << "La longueur moyenne est :" << longueurMoy << "\n";
   
    return 0;

}


