// La Vue-Controlleur pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "CalcWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#pragma pop()
#include <iostream>
#include <type_traits>
#include <cppitertools/range.hpp>

using iter::range;

template <typename T>
QPushButton* CalcWindow::nouveauBouton(const QString& text, const T& slot)
{
	auto bouton = new QPushButton(this);
	bouton->setText(text);
	bouton->setFixedSize(40, 40);
	if constexpr (!is_same_v<T, decltype(nullptr)>)
		QObject::connect(bouton, &QPushButton::clicked, &calc_, slot);
	return bouton;
}

CalcWindow::CalcWindow(QWidget* parent) :
	QMainWindow(parent)
{
	// Si on objet n'a pas encore de parent on lui met "this" comme parent en attendant, si possible, pour s'assurer que tous les pointeurs sont gérés par un delete automatique en tout temps sans utiliser de unique_ptr.
	auto widgetPrincipal = new QWidget(this);
	auto layoutPrincipal = new QVBoxLayout(widgetPrincipal);  // Donner un parent à un layout est comme un setLayout.

	// Pour faire la série de 10 boutons qui doivent tous appeler la même fonction du modèle mais avec des valeurs différentes d'argument, voici trois manières de le faire:
	
	// ------------------------------------------------------------
	// Version avec lambdas:
	{
		auto layout = new QHBoxLayout(); // Pas possible de donner directement le parent au layout (le constructeur prend un QWidget* et un layout n'en est pas un; on ne peut pas mettre un parent qui a déjà un layout; si on met on parent temporaire, addLayout n'accepte pas de changer le parent).
		layoutPrincipal->addLayout(layout);

		layout->setSpacing(0);
		for (int i : range(10))
			layout->addWidget(nouveauBouton(QString::number(i), [this,i]() { calc_.ajouterChiffre(i); }));

		layout->addSpacing(10);
		layout->addWidget(nouveauBouton("+", &Calc::operationPlus));
		layout->addWidget(nouveauBouton("-", &Calc::operationMoins));
		layout->addWidget(nouveauBouton("=", &Calc::operationEgal));

		layout->addSpacing(10);
		auto label = new QLabel(this);
		label->setMinimumWidth(100);
		// On pourrait connecter un slot (on en a un pour l'autre exemple) mais ici c'était simple comme ça et c'est la version avec lambdas.
		QObject::connect(&calc_, &Calc::valeurChangee, this, [label](int valeur) {
			label->setText(QString::number(valeur));
		});
		layout->addWidget(label);
	}

	// ------------------------------------------------------------
	// Version avec QButtonGroup:
	{
		auto layout = new QHBoxLayout();
		layoutPrincipal->addLayout(layout);

		layout->setSpacing(0);
		auto groupeBoutons = new QButtonGroup(this);
		for (int i : range(10)) {
			auto bouton = nouveauBouton(QString::number(i));
			groupeBoutons->addButton(bouton, i); // L'ID du bouton est i (doit être un entier).
			layout->addWidget(bouton);
		}
		#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)  // Le nom du signal idClicked existe depuis Qt 5.15
		QObject::connect(groupeBoutons, &QButtonGroup::idClicked, &calc_, &Calc::ajouterChiffre); // ajouterChiffre prend un int, donc le ID du bouton est bon directement.
		#else
		QObject::connect(groupeBoutons, SIGNAL(buttonClicked(int)), &calc_, SLOT(ajouterChiffre(int)));
		#endif

		layout->addSpacing(10);
		layout->addWidget(nouveauBouton("+", &Calc::operationPlus));
		layout->addWidget(nouveauBouton("-", &Calc::operationMoins));
		layout->addWidget(nouveauBouton("=", &Calc::operationEgal));

		layout->addSpacing(10);
		auto label = new QLabel(this);
		affichage_ = label;
		label->setMinimumWidth(100);
		QObject::connect(&calc_, &Calc::valeurChangee, this, &CalcWindow::changerValeurAffichee);
		layout->addWidget(label);
	}

	// ------------------------------------------------------------
	// Version avec setProperty:
	{
		auto layout = new QHBoxLayout();
		layoutPrincipal->addLayout(layout);

		layout->setSpacing(0);
		for (int i : range(10)) {
			auto bouton = nouveauBouton(QString::number(i));
			// On donne un nom à la propriété, et on lui donne une valeur QVariant (comme dans les notes de cours) d'un type quelconque (doit enregistrer le type avec Q_DECLARE_METATYPE(LeType) si ce n'est pas un type déjà connu de Qt).
			bouton->setProperty("chiffre", QVariant::fromValue<int>(i));
			QObject::connect(bouton, &QPushButton::clicked, this, &CalcWindow::chiffreAppuye);
			layout->addWidget(bouton);
		}

		layout->addSpacing(10);
		layout->addWidget(nouveauBouton("+", &Calc::operationPlus));
		layout->addWidget(nouveauBouton("-", &Calc::operationMoins));
		layout->addWidget(nouveauBouton("=", &Calc::operationEgal));

		// On ne met pas un autre affichage, on en a déjà deux versions différentes.
		layout->addSpacing(110);
	}

	setCentralWidget(widgetPrincipal);
	setWindowTitle("Calculatrice simple");
}


// Pour la version QButtonGroup.
// Pourrait aussi être sans paramètre et faire calc_.obtenirValeur()
void CalcWindow::changerValeurAffichee(int valeur)
{
	affichage_->setText(QString::number(valeur));
}


// Pour la version setProperty.
void CalcWindow::chiffreAppuye()
{
	// QObject::sender() est l'objet d'où vient le signal connecté à ce slot; attention qu'il sera nullptr si le slot est appelé directement au lieu de passer par un signal.
	calc_.ajouterChiffre(QObject::sender()->property("chiffre").value<int>());
}
