/**
* Programme qui charge une grille de Tic-Tac-Toe depuis un fichier texte et renvoie le vainqueur de la partie
* \file   Exo5.cpp
* \author Maya Teylouni et Julien Métais
* \date   26 janvier 2021
* Créé le 26 janvier 2021
*/

#include<iostream>
#include<fstream>

using namespace std;

// Fonction qui indique si le joueur qui utilise le symbole passé en argument a remporté la partie représentée par la grille.
bool verifierVictoire(char grille[], char symbole){
    bool estVictorieux = false;

    // On cherche d'abord d'éventuelles lignes gagnantes.
    if (grille[0] == symbole && grille[1] == symbole && grille[2] == symbole){
        estVictorieux = true;
    }
    if (grille[3] == symbole && grille[4] == symbole && grille[5] == symbole) {
        estVictorieux = true;
    }
    if (grille[6] == symbole && grille[7] == symbole && grille[8] == symbole) {
        estVictorieux = true;
    }

    // Puis on examine les colonnes.
    if (grille[0] == symbole && grille[3] == symbole && grille[6] == symbole) {
        estVictorieux = true;
    }
    if (grille[1] == symbole && grille[4] == symbole && grille[7] == symbole) {
        estVictorieux = true;
    }
    if (grille[2] == symbole && grille[5] == symbole && grille[8] == symbole) {
        estVictorieux = true;
    }

    // Et enfin les diagonales.
    if (grille[0] == symbole && grille[4] == symbole && grille[8] == symbole) {
        estVictorieux = true;
    }
    if (grille[2] == symbole && grille[4] == symbole && grille[6] == symbole) {
        estVictorieux = true;
    }

    return estVictorieux;
}

int main() {
    // Saisie au clavier du nom du fichier texte à lire.
    cout << "Saisir le nom du fichier à lire" << endl;
    string nomDuFichier = "";
    cin >> nomDuFichier;

    // Lecture du fichier et écriture de la grille.
    ifstream lecteur;
    lecteur.open(nomDuFichier);

    char grille[9] = {}; //Le tableau contenant l'état de la partie.
    if (lecteur.is_open()) {
        for (int i = 0; i < 9; i++) {
            lecteur >> grille[i];
        }
    }
    lecteur.close();

    // On vérifie si le joueur 1 et/ou le joueur 2 a remporté la partie, puis on affiche un message en conséquence.
    bool estRemporteParJoueur1 = verifierVictoire(grille, 'x');
    bool estRemporteParJoueur2 = verifierVictoire(grille, 'o');

    if (estRemporteParJoueur1) {
        if (!estRemporteParJoueur2) {
            cout << "Le joueur 1 gagne." << endl;
        }
        else {
            cout << "Incohérence dans la grille." << endl;
        }
    }
    else {
        if (estRemporteParJoueur2) {
            cout << "Le joueur 2 gagne." << endl;
        }
        else {
            cout << "Egalité" << endl;
        }
    }

    return 0;
}