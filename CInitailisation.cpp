#include "CInitailisation.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CCharacter.h"
#include "CMatchs.h"
#include "CTournament.h"

using namespace std;
using namespace Matchs;
using namespace Tournament;

namespace Initialisation
{
    vector <CCharacter> Init (unsigned NbJoueur, vector <vector <string>> MPerso, vector <string> NomJoueur)
    {
        vector <CCharacter> J;
        for (unsigned i (0); i < NbJoueur; ++i) //création de tous les personages pour chacun des joueurs
            for (unsigned j (0); j < MPerso[i].size(); ++j)
                J.push_back(CCharacter (MPerso[i][j], i + 1, NomJoueur[i], "Winner", 0, j));
        return J;
    }

    vector <vector <string>> InitPersoChoix (unsigned NbJoueur, vector <string> NomJoueur)
    {
        vector <vector <string>> MPerso;
        vector <string> VPerso;
        for (unsigned i (0); i < NbJoueur; ++i)
        {
            system("cls");
            cout << NomJoueur[i] << " : " << endl;
            string Quitter = "q";
            string Perso;
            if (i == 0)
                cin.ignore();
            getline(cin, Perso);
            while (Perso != Quitter)
            {
                if (Perso == "show") //Afficher la liste de perso
                    for (unsigned j (0); j < VPerso.size(); j++)
                        cout << VPerso[j] << endl;
                if (Perso == "-all") //Effacer toutes la liste de perso
                    while (VPerso.size() > 0)
                        VPerso.pop_back();
                if (Perso[0] == '-' && Perso != "-all") //Pour supprimer un perso de la liste
                    for (unsigned j (0); j < VPerso.size(); j++)
                        if (VPerso[j] == Perso.substr(1,Perso.size() - 1)) //Recherche du perso voulu (On enlève le '-')
                            VPerso.erase(VPerso.begin()+j); //Effacement du perso de la liste
                if (Perso == "all") //Pour ajouter tous les perso d'un coup
                {
                    VPerso.push_back("Bowser");
                    VPerso.push_back("Bayonetta");
                    VPerso.push_back("Bowser Jr");
                    VPerso.push_back("Captain Falcon");
                    VPerso.push_back("Charizard");
                    VPerso.push_back("Cloud");
                    VPerso.push_back("Corin");
                    VPerso.push_back("Dark Pit");
                    VPerso.push_back("Diddy Kong");
                    VPerso.push_back("Donkey Kong");
                    VPerso.push_back("Dr. Mario");
                    VPerso.push_back("Duck Hunt");
                    VPerso.push_back("Falco");
                    VPerso.push_back("Fox");
                    VPerso.push_back("Ganondorf");
                    VPerso.push_back("Greninja");
                    VPerso.push_back("Ike");
                    VPerso.push_back("Jigglypuff");
                    VPerso.push_back("King Dedede");
                    VPerso.push_back("Kirby");
                    VPerso.push_back("Link");
                    VPerso.push_back("Little Mac");
                    VPerso.push_back("Lucario");
                    VPerso.push_back("Lucas");
                    VPerso.push_back("Lucina");
                    VPerso.push_back("Luigi");
                    VPerso.push_back("Mario");
                    VPerso.push_back("Marth");
                    VPerso.push_back("Mega Man");
                    VPerso.push_back("Mewtwo");
                    VPerso.push_back("Mii Brawler");
                    VPerso.push_back("Mii Gunner");
                    VPerso.push_back("Mii Swordsman");
                    VPerso.push_back("Mr. Game & Watch");
                    VPerso.push_back("Ness");
                    VPerso.push_back("Pac-Man");
                    VPerso.push_back("Palutena");
                    VPerso.push_back("Peach");
                    VPerso.push_back("Pikachu");
                    VPerso.push_back("Pit");
                    VPerso.push_back("R.O.B.");
                    VPerso.push_back("Robin");
                    VPerso.push_back("Rosaluma");
                    VPerso.push_back("Roy");
                    VPerso.push_back("Ryu");
                    VPerso.push_back("Samus");
                    VPerso.push_back("Sheik");
                    VPerso.push_back("Shulk");
                    VPerso.push_back("Sonic");
                    VPerso.push_back("Toon Link");
                    VPerso.push_back("Villager");
                    VPerso.push_back("Wario");
                    VPerso.push_back("Wii Fit Trainer");
                    VPerso.push_back("Yoshi");
                    VPerso.push_back("Zelda");
                    VPerso.push_back("Zero Suit Samus");
                }
                if (Perso != "" && Perso != "all" && Perso[0] != '-' && Perso != "show") //Si le texte n'est pas vide, que ce n'est pas un demande de vue de la liste, que ce n'est pas un ajout global et que ce n'est pas une suppresion
                    VPerso.push_back(Perso); //On ajoute le perso dans la liste
                //cout << "perso ? (q pour quitter) : ";
                getline (cin, Perso);

            }
            MPerso.push_back(VPerso); //On ajoute la liste de perso au tableau Joueurs/Perso
            VPerso.clear(); //On vide la liste de perso du joueur en cour pour faire celle du joueur suivant
        }
        return MPerso;
    }

    vector <vector <string>> InitPerso (unsigned NbJoueur)
    {
        vector <vector <string>> MPerso;
        vector <string> VPerso;
        //Création de la liste de tous les perso
        VPerso.push_back("Bowser");
        VPerso.push_back("Bayonetta");
        VPerso.push_back("Bowser Jr");
        VPerso.push_back("Captain Falcon");
        VPerso.push_back("Charizard");
        VPerso.push_back("Cloud");
        VPerso.push_back("Corin");
        VPerso.push_back("Dark Pit");
        VPerso.push_back("Diddy Kong");
        VPerso.push_back("Donkey Kong");
        VPerso.push_back("Dr. Mario");
        VPerso.push_back("Duck Hunt");
        VPerso.push_back("Falco");
        VPerso.push_back("Fox");
        VPerso.push_back("Ganondorf");
        VPerso.push_back("Greninja");
        VPerso.push_back("Ike");
        VPerso.push_back("Jigglypuff");
        VPerso.push_back("King Dedede");
        VPerso.push_back("Kirby");
        VPerso.push_back("Link");
        VPerso.push_back("Little Mac");
        VPerso.push_back("Lucario");
        VPerso.push_back("Lucas");
        VPerso.push_back("Lucina");
        VPerso.push_back("Luigi");
        VPerso.push_back("Mario");
        VPerso.push_back("Marth");
        VPerso.push_back("Mega Man");
        VPerso.push_back("Mewtwo");
        VPerso.push_back("Mii Brawler");
        VPerso.push_back("Mii Gunner");
        VPerso.push_back("Mii Swordsman");
        VPerso.push_back("Mr. Game & Watch");
        VPerso.push_back("Ness");
        VPerso.push_back("Pac-Man");
        VPerso.push_back("Palutena");
        VPerso.push_back("Peach");
        VPerso.push_back("Pikachu");
        VPerso.push_back("Pit");
        VPerso.push_back("R.O.B.");
        VPerso.push_back("Robin");
        VPerso.push_back("Rosaluma");
        VPerso.push_back("Roy");
        VPerso.push_back("Ryu");
        VPerso.push_back("Samus");
        VPerso.push_back("Sheik");
        VPerso.push_back("Shulk");
        VPerso.push_back("Sonic");
        VPerso.push_back("Toon Link");
        VPerso.push_back("Villager");
        VPerso.push_back("Wario");
        VPerso.push_back("Wii Fit Trainer");
        VPerso.push_back("Yoshi");
        VPerso.push_back("Zelda");
        VPerso.push_back("Zero Suit Samus");
        for (unsigned i (0); i < NbJoueur; i++)
            MPerso.push_back(VPerso); //ajout de la liste de perso dans le tableau Joueur/Perso
        return MPerso;

    }

    vector <string> InitNomJoueur (unsigned NbJoueur)
    {
        cin.ignore();
        vector <string> NomJoueur;
        for (unsigned i (0); i < NbJoueur; ++i) //Nommer les Joueurs
        {
            cout << "Joueur " << i+1 << " : ";
            string Name;
            getline (cin, Name);
            NomJoueur.push_back(Name);
        }
        return NomJoueur;
    }

    vector <string> InitNomJoueurBase(unsigned NbJoueur)
    {
        vector <string> NomJoueur;
        for (unsigned i (0); i < NbJoueur; ++i)
        {
            NomJoueur.push_back("Joueur " + to_string(i + 1)); //Les joueurs s'appelle Joueur x
        }
        return NomJoueur;
    }

    void StatsJoueurConnus (vector <CCharacter> J, vector <string> JoueurConnus)
    {
        for (unsigned i = 0; i < JoueurConnus.size(); ++i)
        {
            vector <unsigned> VNbMatchs;
            vector <string> VPerso;
            vector <float> VNbVictoire;
            string NbVictoire;
            string NbMatchs;
            string Perso = "";
            ifstream StatsFileRead ("Stats/MaJ/" + JoueurConnus[i] + ".txt");
            string Vide;
            getline(StatsFileRead, Perso); //on lit la ligne
            while (Perso != "")
            {
                VPerso.push_back(Perso); //on ajoute le perso dans la liste de perso
                getline(StatsFileRead, NbMatchs); //on lit la ligne suivante
                VNbMatchs.push_back(stof(NbMatchs)); //on ajoute le nombre de matchs du perso dans sa liste
                getline(StatsFileRead, NbVictoire);
                VNbVictoire.push_back(stof(NbVictoire));
                getline(StatsFileRead, Vide); //on lit la ligne vide
                getline(StatsFileRead, Perso); //on lit la ligne suivante
            }

            for (unsigned j = 0; j < J.size(); ++j)
            {
                if (J[j].NomJoueur() == JoueurConnus[i]) //Si le nom du joueur dans le vecteur J est un nom de la liste des joueurs
                {
                    if (J[j].GetNbMatch() > 0) //Et le nombre de match fait avec cette combinaison est superieur à 0
                    {
                        bool In = false;
                        for (unsigned k = 0; k < VPerso.size(); ++k)
                        {
                            if (VPerso[k] == J[j].GetChara()) //Si le perso de la combinaison est dans la liste de perso du .txt
                            {
                                VNbMatchs[k] += J[j].GetNbMatch(); //On ajoute le nombre de matchs au nombre de matchs déjà fait
                                VNbVictoire[k] += J[j].GetNbVictoire();
                                In = true;
                            }
                        }

                        if (!In) //sinon, on les ajoute aux listes
                        {
                            VPerso.push_back(J[j].GetChara());
                            VNbMatchs.push_back(J[j].GetNbMatch());
                            VNbVictoire.push_back(J[j].GetNbVictoire());
                        }
                    }
                }
            }
            ofstream StatsFile("Stats/MaJ/" + JoueurConnus[i] + ".txt");
            for (unsigned j = 0; j < VPerso.size(); ++j)
                StatsFile << VPerso[j] << endl << VNbMatchs[j] << endl << VNbVictoire[j] << endl << endl;
            StatsFile.close();
            ofstream StatsFile2("Stats/" + JoueurConnus[i] + ".txt");
            for (unsigned j = 0; j < VPerso.size(); ++j)
                StatsFile2 << VNbMatchs[j] << " matchs dont " << VNbVictoire[j] << " victoire (" << (int) ((VNbVictoire[j] / VNbMatchs[j]) * 100)
                           << "%) avec " << VPerso[j] << endl << endl;
        }
    }

    void Launch ()
    {
        cout << "Nombre de joueurs (2 minimum) : ";
        unsigned NbJoueur;
        cin >> NbJoueur;
        char choixConnus;
        cout << "Joueurs ayant déjà leurs option ? (y/n) : ";
        cin >> choixConnus;
        vector <string> JoueurConnus;
        vector <string> PersoConnus;
        vector <vector <string>> JoueurConnusListe;
        switch (choixConnus)
        {
            case 'y':
            {
                string Name;
                string Perso;
                for (unsigned i (0); i < NbJoueur; ++i)
                {
                    cout << "Joueur : ";
                    cin >> Name;

                    if (Name == "q") break;

                    ifstream Joueur ("Profils/" + Name + ".txt");

                    if(getline(Joueur, Perso)) //Verification du nom : s'il le fichier est vide, alors le fichier n'existe pas
                        JoueurConnus.push_back(Name);
                    Joueur.close();

                    ifstream Joueurs ("Profils/" + Name + ".txt");

                    while (getline(Joueurs, Perso)) //Tant qu'on est pas à la dernière ligne, on ajoute le perso au vecteur
                        PersoConnus.push_back(Perso);
                    Joueurs.close();

                    system("cls");

                    if (PersoConnus.size() > 0)
                        JoueurConnusListe.push_back(PersoConnus); //On ajoute le vecteur de perso à la matrice joueur/perso
                    else
                        i--;

                    while (PersoConnus.size() > 0)
                        PersoConnus.pop_back();
                }
                break;
            }
        }

        char ChoixJoueur;
        if (JoueurConnus.size() != NbJoueur) //Si tous les joueurs sont déjà rentré, on ne fait pas ça
        {
            cout << "Nommer les joueurs ? (y/n) : ";
            cin >> ChoixJoueur;
        }
        vector <string> NomJoueurs;
        switch (ChoixJoueur)
        {
            case 'y' :
                NomJoueurs = InitNomJoueur(NbJoueur - JoueurConnus.size()); //On initialise le vecteur avec InitNomJoueur avec un nombre de joueurs restant
                for (unsigned i (0); i < JoueurConnus.size(); ++i)
                    NomJoueurs.push_back(JoueurConnus[i]); //On ajoute les joueurs déjà rentré dans le vecteur
                break;
            default :
                NomJoueurs = InitNomJoueurBase(NbJoueur - JoueurConnus.size());
                for (unsigned i (0); i < JoueurConnus.size(); ++i)
                    NomJoueurs.push_back(JoueurConnus[i]);
                break;
        }
        char ChoixPerso;
        if (JoueurConnus.size() != NbJoueur)
        {
            cout << "Choix des persos ? (y/n) : ";
            cin >> ChoixPerso;
        }
        vector <CCharacter> J;
        vector <vector <string>> ListePerso;
        switch (ChoixPerso)
        {
            case 'y' :
                ListePerso = InitPersoChoix(NbJoueur - JoueurConnus.size(), NomJoueurs); //De même que ci-dessus
                for (unsigned i (0); i < JoueurConnusListe.size(); ++i)
                    ListePerso.push_back(JoueurConnusListe[i]);
                break;
            default :
                ListePerso = InitPerso(NbJoueur - JoueurConnus.size());
                for (unsigned i (0); i < JoueurConnusListe.size(); ++i)
                    ListePerso.push_back(JoueurConnusListe[i]);
                break;
        }
        J = Init(NbJoueur, ListePerso, NomJoueurs);
        cout << "Type de matchs (1 = 1v1, 2 = 2v2, 3 = 3v3, 4 = 4v4, 5 = FFA, 6 = 2v2 x 3," << endl << "7 = 2v2 x 4, 8 = Random, 9 = Tournoi Single Elimination) : ";
        unsigned TypeTourn;
        cin >> TypeTourn;
        bool Finish = false;
        while (!Finish) //Verification du Nombre de joueurs. S'il ne sont pas assez, on affiche un message et on revient à demander le type de match
        {
            switch (TypeTourn)
            {
                case 1 :
                    if (NbJoueur < 2)
                        break;
                    Finish = true;
                    break;
                case 2 :
                    if (NbJoueur < 4)
                        break;
                    Finish = true;
                    break;
                case 3 :
                    if (NbJoueur < 6)
                        break;
                    Finish = true;
                    break;
                case 4 :
                    if (NbJoueur < 8)
                        break;
                    Finish = true;
                    break;
                case 5 :
                    if (NbJoueur < 2)
                        break;
                    Finish = true;
                    break;
                case 6 :
                    if (NbJoueur < 6)
                        break;
                    Finish = true;
                    break;
                case 7 :
                    if (NbJoueur < 8)
                        break;
                    Finish = true;
                    break;
                case 8 :
                    if (NbJoueur < 2)
                        break;
                    Finish = true;
                    break;
                case 9 :
                    if (NbJoueur < 2)
                        break;
                    Finish = true;
                    break;
                default :
                    break;
            }
            system("cls");
            if (!Finish)
            {
                system("cls");
                cout << "/!\\ " << NbJoueur << " joueurs sont insuffisant pour ce mode /!\\" << endl << endl;
                cout << "Type de matchs (1 = 1v1, 2 = 2v2, 3 = 3v3, 4 = 4v4, 5 = FFA, 6 = 2v2 x 3," << endl << "7 = 2v2 x 4, 8 = Random, 9 = Tournois Single Elimination) : ";
                cin >> TypeTourn;
            }
        }
        unsigned NbMatch;
        if (TypeTourn != 9)
        {
            cout << "Nombre de matchs : ";
            cin >> NbMatch;
        }
        switch (TypeTourn)
        {
            case 1 :
                M1v1(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 2 :
                M2v2(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 3 :
                M3v3(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 4 :
                M4v4(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 5 :
                FFA(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 6 :
                M2v2x3(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 7 :
                M2v2x4(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 8 :
                RandomMatchType(J, NbJoueur, NbMatch, ListePerso);
                break;
            case 9:
                Tournament1v1SE(J, NbJoueur, ListePerso);
            default :
                break;
        }
        if (TypeTourn != 9)
        {
            Stats(J, NbJoueur, ListePerso);
            StatsJoueurConnus(J, NomJoueurs);
        }
        return;
    }
}
