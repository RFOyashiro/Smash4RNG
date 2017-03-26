#include "WinNommerjoueur.h"

WinNommerJoueur::WinNommerJoueur(bool Perso, unsigned NbJoueur)
    : ChoisirPerso (Perso), m_NbJoueur (NbJoueur)
{
    QMainWindow Window;

    QWidget CentralWid;

    QFormLayout Form;

    for (unsigned i (0); i < NbJoueur; ++i)
    {
        QLabel label;
        std::string str = "Joueur " + std::to_string(i);
        label.setText(QString::fromStdString(str));
        QWidget widget;
        QLineEdit line (&widget);
        line.setText(QString::fromStdString(str));
        Form.addRow(&label, &widget);
    }

    CentralWid.setLayout(&Form);
    //CentralWid.setParent(&Window);
    CentralWid.show();

}
