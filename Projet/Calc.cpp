// Le Modèle pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Calc.hpp"

// slots:

void Calc::changerValeur(int valeur) {
	if (valeur != valeur_) {
		valeur_ = valeur;
		emit valeurChangee(valeur_);
	}
}
void Calc::effacer() {
	operation_ = egal; changerValeur(0);
}
void Calc::ajouterChiffre(int chiffre) {
	if (estResultat_) {
		valeur_ = 0;
		estResultat_ = false;
	}
	changerValeur(valeur_ * 10 + chiffre);
}

void Calc::changerOperation(const function<int(int, int)>& operation) {
	if (!estResultat_)
		effectuerOperation();
	operation_ = operation;
}

void Calc::operationPlus()  { changerOperation(plus); }
void Calc::operationMoins() { changerOperation(moins); }
void Calc::operationEgal()  { changerOperation(egal); }

void Calc::effectuerOperation() {
	resultatPrecedent_ = operation_(resultatPrecedent_, valeur_);
	estResultat_ = true;
	changerValeur(resultatPrecedent_);
}

// Fonctions pour l'opération:
// (on aurait pu à la place mettre des lambdas dans operationPlus() ...)
int Calc::plus (int x, int y) { return x + y; }
int Calc::moins(int x, int y) { return x - y; }
int Calc::egal (int  , int y) { return y; }
