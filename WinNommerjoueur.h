#ifndef WINNOMMERJOUEUR_H
#define WINNOMMERJOUEUR_H

#include <QFormLayout>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QString>


class WinNommerJoueur
{
    private:
        bool ChoisirPerso;
        unsigned m_NbJoueur;

    public:
        WinNommerJoueur(bool Perso = false, unsigned NbJoueur = 0);

};

#endif // WINNOMMERJOUEUR_H
