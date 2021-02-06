/**
* Programme qui affiche le mot le plus court, le mot le plus long et la longueur moyenne des mots d'une phrase saisie au clavier
* \file   Exo1.cpp
* \author Maya Kurdi-Teylouni et Julien Métais
* \date   5 février 2021
* Créé le 26 janvier 2021
*/

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    string phrase;
    std::cout << "Ecrivez une phrase svp\n"; // L'utilisateur écrit une phrase.
    getline(cin, phrase);
    
    string motCourt = phrase;
    string motLong = "";
    string motCourant = "";
    double longueurTotale = 0.0;
    int nMots = 0;
    double longueurMoyenne = 0.0;

    // On passe chaque caractère de la phrase :
    for (int i = 0; i < (int)phrase.length(); i++)
    {
        int result = isspace(phrase[i]);
        if (result == 0) // Si le caractère courant est un espace :
        {
            motCourant += phrase[i];
        }
        if (result == 8 || i == (int)phrase.length() - 1) // Si le caractère actuel n'est pas un espace ou si on est au dernier caractère de la phrase :
        {
            if (motCourant.length() < motCourt.length()) // Mise à jour de la valeur du mot le plus court de la phrase.
            {
                motCourt = motCourant;
            }
            if (motCourant.length() > motLong.length()) // Mise à jour de la valeur du mot le plus long de la phrase.
            {
                motLong = motCourant;
            }

            longueurTotale += motCourant.length();
            nMots += 1;
            motCourant = "";

            if (nMots != 0) // Calcul de la longueur moyenne des mots dans la phrase.
            {
                longueurMoyenne = longueurTotale / double(nMots);
            }    
        }
    }

    //Affichage du mot le plus court, du mot le plus  long et de la longueur moyenne
    std::cout << "Le mot le plus court est : " << motCourt << "\n";
    std::cout << "Le mot le plus long est : " << motLong << "\n";
    std::cout << "La longueur moyenne est : " << longueurMoyenne << "\n";
   
    return 0;

}


