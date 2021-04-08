// Les test pour le Modèle pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Calc.hpp"

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif
#ifdef TEST

TEST(Calc, simple) {
	Calc calc;
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(4);
	calc.ajouterChiffre(3);
	EXPECT_EQ(calc.obtenirValeur(), 143);
	calc.effacer();
	calc.ajouterChiffre(2);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(8);
	EXPECT_EQ(calc.obtenirValeur(), 218);
}

TEST(Calc, operations) {
	Calc calc;
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(4);
	calc.ajouterChiffre(3);
	calc.operationPlus();
	calc.ajouterChiffre(2);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(8);
	calc.operationEgal();
	EXPECT_EQ(calc.obtenirValeur(), 143+218);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(4);
	calc.ajouterChiffre(3);
	calc.operationMoins();
	calc.ajouterChiffre(2);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(8);
	calc.operationEgal();
	EXPECT_EQ(calc.obtenirValeur(), 143-218);
}

TEST(Calc, changement_operation) {
	Calc calc;
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(4);
	calc.ajouterChiffre(3);
	calc.operationPlus();
	calc.operationMoins();
	calc.ajouterChiffre(2);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(8);
	calc.operationEgal();
	EXPECT_EQ(calc.obtenirValeur(), 143-218);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(4);
	calc.ajouterChiffre(3);
	calc.operationMoins();
	calc.operationPlus();
	calc.ajouterChiffre(2);
	calc.ajouterChiffre(1);
	calc.ajouterChiffre(8);
	calc.operationEgal();
	EXPECT_EQ(calc.obtenirValeur(), 143+218);
}

#endif
