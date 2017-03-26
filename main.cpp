#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CCharacter.h"
#include "CMatchs.h"
#include "CInitailisation.h"
#include "WinItialisation.h"
#include <QApplication>

//ToDo : Meilleur vérification des Users Inputs -> exemple : si NbJoueur < NbJoueur necessaire, empecher l'avancement et alerter
//ToDo : Si un résultat est faux (le joueur n'est pas dans le match), réduire le nombre de match des 2 joueurs et inscrire "aucun gagnant" dans le
//       fichier Resultats/Matchs.txt
//ToDo : IHM !!!! => Gros changement : création liste perso, génération complet des matchs, classement après chaque matchs, TROUVER LA DOC
//       (cf carnet pour design) => Le cancer est réel !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ToDo : Retrouver comment faire une lib
//Ques : IHM ? oO oO oO o0 oO oO Oo OO oo oO O oOoOO

using namespace std;
using namespace Initialisation;


int main (/*int argc, char *argv[]*/)
{
    srand(time(NULL));
    ofstream Resultats ("Resultats/Matchs.txt");
    Resultats.close();
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();*/
    Launch();
    return 0;
}
