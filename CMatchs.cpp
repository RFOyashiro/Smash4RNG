#include "CMatchs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include "CCharacter.h"

using namespace std;
namespace Matchs
{
    void Stats (vector <CCharacter> J, unsigned NbJoueur, vector <vector <string>> MPerso)
    {
        vector <unsigned> VNbPerso ;
        for (unsigned i (0); i < MPerso.size(); ++i)
            VNbPerso.push_back(MPerso[i].size());


        ofstream fichierMatchJoueurPerso ("Resultats/Joueur-Perso.txt");
        for (unsigned i (0); i < J.size(); ++i)
        {
            if (i > 0 && J[i].GetPlayer() != J[i - 1].GetPlayer()) //Si i > 0 alors Si le joueur de la combinaison actuelle est différent du joueur de la combinaison précédents
                fichierMatchJoueurPerso << "***************************************" << endl; //séparation des différents joueurs
            if (J[i].GetNbMatch() > 0)
                fichierMatchJoueurPerso << J[i].NomJoueur() << " a fait " << J[i].GetNbMatch() << " matchs avec " << J[i].GetChara() << endl
                                        << J[i].GetNbVictoire() << " Victoires (" << (J[i].GetNbVictoire() * 100) / J[i].GetNbMatch() << "%)" << endl << endl;
        }
        ofstream fichierMatchTotalJoueur ("Resultats/Classement.txt");
        ofstream fichierMatchTotalJoueurPond ("Resultats/ClassementPond.txt");
        vector <vector <unsigned>> MatMatchs (NbJoueur);
        for (unsigned i (0); i < NbJoueur; ++i)
        {
            vector <unsigned> Init (4);
            MatMatchs[i] = Init;
            MatMatchs[i][0] = i + 1; //NumJoueur
            MatMatchs[i][1] = 0;     //NbMatchs
            MatMatchs[i][2] = 0;     //NbVictoires
            MatMatchs[i][3] = 0;	 //Ratio Victoires/Matchs
        }
        for (unsigned i (0); i < MatMatchs.size(); ++i)
        {
            for (unsigned j (0); j < J.size(); ++j)
            {
                if (J[j].GetPlayer() == MatMatchs[i][0])
                {
                    MatMatchs[i][1] += J[j].GetNbMatch(); //calcul du nombre total de match
                    MatMatchs[i][2] += J[j].GetNbVictoire(); //calcul du nombre total de victoire
                    if (MatMatchs[i][1] > 0)
                        MatMatchs[i][3] = (MatMatchs[i][2] * 100) / MatMatchs[i][1]; //calcul du ratio victoires/matchs
                }
            }
        }
        for (unsigned i (0); i < MatMatchs.size(); ++i)
        {
            for (unsigned j (i+1); j < MatMatchs.size(); ++j)
            {
                if (i == MatMatchs.size() - 1) break;
                if (MatMatchs[i][3] < MatMatchs[j][3])
                {
                    swap(MatMatchs[i],MatMatchs[j]); //classement par ratio
                    swap(VNbPerso[i], VNbPerso[j]);
                }
                else if (MatMatchs[i][1] > MatMatchs[j][1] && MatMatchs[i][3] == MatMatchs[j][3])
                {
                    swap(MatMatchs[i], MatMatchs[j]); //calssement par nombre de défaites si les ratios sont identique
                    swap(VNbPerso[i], VNbPerso[j]);
                }
            }
            if (MatMatchs[i][1] != 0)
            {
                string Name;
                for (unsigned j (0); j < J.size(); ++j)
                    if (J[j].GetPlayer() == MatMatchs[i][0])
                    {
                        Name = J[j].NomJoueur(); //recuperation du nom du joueur
                        break;
                    }
                fichierMatchTotalJoueur << i + 1 << "° : " << Name << " a fait " << MatMatchs[i][1] << " matchs" << endl
                                        << "avec " << MatMatchs[i][2] << " victoires (" << MatMatchs[i][3] << "%)" << endl << endl;
            }
        }

        unsigned BestPourcentage = 0;
        unsigned BestMatch = 0;
        unsigned BestVictoire = 0;
        unsigned BestNbPers = 0;

        for (unsigned i (0); i < MatMatchs.size(); ++i)
        {
            if (MatMatchs[i][1] > BestMatch)
                BestMatch = MatMatchs[i][1];

            if (MatMatchs[i][2] > BestVictoire)
                BestVictoire = MatMatchs[i][2];

            if (MatMatchs[i][3] > BestPourcentage)
                BestPourcentage = MatMatchs[i][3];

            if (VNbPerso[i] > BestNbPers)
                BestNbPers = VNbPerso[i];
        }

        vector <vector <float>> MatPond (NbJoueur);

        for (unsigned i = 0; i < MatMatchs.size(); ++i)
        {
            MatPond[i].push_back(MatMatchs[i][0]);
            MatPond[i].push_back(((float)MatMatchs[i][1]/BestMatch) * 0.33);
            MatPond[i].push_back(((float)MatMatchs[i][2]/BestVictoire) * 0.33);
            MatPond[i].push_back(((float)MatMatchs[i][3]/BestPourcentage) * 0.33);
            MatPond[i].push_back(((float)VNbPerso[i]/BestNbPers) * 0.01);
        }

        vector <vector <float>> MatRes (NbJoueur);

        for (unsigned i = 0; i < MatPond.size(); ++i)
        {
            MatRes[i].push_back(MatPond[i][0]);
            MatRes[i].push_back(MatPond[i][1]+MatPond[i][2]+MatPond[i][3]+MatPond[i][4]);
        }

        for (unsigned i (0); i < MatMatchs.size(); ++i)
        {
            for (unsigned j (i+1); j < MatMatchs.size(); ++j)
            {
                if (i == MatRes.size() - 1) break;
                if (MatRes[i][1] < MatRes[j][1])
                    swap(MatRes[i],MatRes[j]);
            }
            string Name;
            for (unsigned j (0); j < J.size(); ++j)
                if (J[j].GetPlayer() == MatRes[i][0])
                {
                    Name = J[j].NomJoueur(); //recuperation du nom du joueur
                    break;
                }
            fichierMatchTotalJoueurPond << i + 1 << "° : " << Name << " a un score de " << fixed << setprecision(3) << MatRes[i][1]  << endl << endl;

        }
    }

    void M1v1 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        vector <pair <CCharacter, CCharacter>> Matches;
        if (NbJoueur < 2) return;
        string Res = "reroll";
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        ofstream ChangeLog ("Log.txt", ios_base::app);
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {

            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1); //augmentaion du numero de matchs
            }
            while (Res == "reroll")
            {
                system("cls");
                unsigned J2;
                unsigned J1;
                J1 = rand() % NbJoueur + 1; //choix des joueurs pour le match
                J2 = rand() % NbJoueur + 1;
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1; //verification si 2 fois le même joueur et changement si la cas est
                unsigned P1 = rand() % MPerso[J1 - 1].size(); //choix des perso pour le matchs pour chaque joueurs
                unsigned P2 = rand() % MPerso[J2 - 1].size();
                pair <CCharacter, CCharacter> Match;
                string Chieur = "Uaethar";
                for (int i (0); i < J.size(); ++i)
                {
                    if ((J[i].GetPlayer() == J1 && J[i].GetNumPerso() == P1))
                    {
                        if (J[i].NomJoueur() == Chieur)
                        {
                            J1 = rand() % NbJoueur + 1;
                            while (J1 == J2)
                                J1 = rand() % NbJoueur + 1;
                            P1 = rand() % MPerso[J1 - 1].size();
                            i = -1;
                        }
                        else
                        {
                            if (Listing) J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first = J[i]; //cherche le joueur et perso choisi
                            break;
                        }
                    }
                }
                for (int i (0); i < J.size(); ++i)
                {
                    if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                    {
                        if (J[i].NomJoueur() == Chieur)
                        {
                            J2 = rand() % NbJoueur + 1;
                            while (J1 == J2)
                                J2 = rand() % NbJoueur + 1;
                            P2 = rand() % MPerso[J2 - 1].size();
                            i = -1;
                        }
                        else
                        {
                            if (Listing) J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second = J[i]; //cherche le joueur et perso choisi
                            break;
                        }
                    }
                }
                if (!Listing)
                {
                    cout << endl;
                    cout << "Match #" << Match.first.GetRound() << " (1v1) :" << endl;
                    cout << Match.first.GetJoueur() << " " << Match.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.GetJoueur() << " " << Match.second.GetChara() << endl << endl;
                    Resultat << "Match #" << Match.first.GetRound() << " (1v1) :" << endl << endl;
                    Resultat << Match.first.NomJoueur() << " " << Match.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.NomJoueur() << " " << Match.second.GetChara() << endl << endl;

                    cout << "Joueur gagnant : ";
                    cin >> Res;
                    bool Done = false;
                    bool Exists = false;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Res == "reroll")
                        {
                            Resultat << "/!\\ Re-Rolled /!\\" << endl << endl << endl;
                            break;
                        }
                        else if (!Done)
                        {
                            Done = true;
                            for (unsigned i (0); i < J.size(); ++i)
                            {
                                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                                {
                                    J[i].SetNbMatch(J[i].GetNbMatch() + 1); //augmentation du nombre de matchs
                                    break;
                                }
                            }
                            for (unsigned i (0); i < J.size(); ++i)
                            {
                                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                                {
                                    J[i].SetNbMatch(J[i].GetNbMatch() + 1); //augmentation du nombre de matchs
                                    break;
                                }
                            }
                        }

                        if (stof(Res) == J1)
                            if ((J[i].GetPlayer() == stof(Res) && J[i].GetNumPerso() == Match.first.GetNumPerso()))
                            {
                                Exists = true;
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire



                                ChangeLog << to_string(now->tm_year+1900) << '-'
                                           << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                           << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                           << setfill('0') << setw(2) << now->tm_min << ":"
                                           << setfill('0') << setw(2) << now->tm_sec << " - "
                                           << Match.first.GetChara() << " (" << Match.first.NomJoueur() << ") a vaincu "
                                           << Match.second.GetChara() << " (" << Match.second.NomJoueur() << ")" << endl << "*" << endl;
                            }
                        if (stof(Res) == J2)
                            if ((J[i].GetPlayer() == stof(Res) && J[i].GetNumPerso() == Match.second.GetNumPerso()))
                            {
                                Exists = true;
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire

                                ChangeLog << to_string(now->tm_year+1900) << '-'
                                           << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                           << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                           << setfill('0') << setw(2) << now->tm_min << ":"
                                           << setfill('0') << setw(2) << now->tm_sec << " - "
                                           << Match.second.GetChara() << " (" << Match.second.NomJoueur() << ") a vaincu "
                                           << Match.first.GetChara() << " (" << Match.first.NomJoueur() << ")" << endl << "*" << endl;
                            }
                    }
                    if (!Exists)
                    {
                        Resultat << "Aucun Vainqueur / Mauvaise entrée lors de l'écriture du résultat" << endl << endl
                                 << "*****************************************************************************************" << endl << endl;
                    }
                }
                else
                {
                    Matches.push_back(Match);
                    break;
                }
            }

        }

        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.GetRound() << " : ";
                cout <<  Matches[i].first.NomJoueur() << " VS " << Matches[i].second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            system("cls");
            cout << endl;
            cout << "Match #" << Matches[Suivant].first.GetRound() << " : " << endl;
            cout << Matches[Suivant].first.GetJoueur() << " " <<Matches[Suivant].first.GetChara() << endl
                 << "VS " << endl
                 << Matches[Suivant].second.GetJoueur() << " " <<Matches[Suivant].second.GetChara() << endl << endl;
            Resultat << Matches[Suivant].first.NomJoueur() << " " <<Matches[Suivant].first.GetChara() << endl
                     << "VS" << endl
                     << Matches[Suivant].second.NomJoueur() << " " <<Matches[Suivant].second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Joueur gagnant : ";
            cin >> Gagnant;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if (Gagnant == Matches[Suivant].first.GetPlayer())
                    if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Matches[Suivant].first.GetNumPerso()))
                    {
                        J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                        Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                 << "*****************************************************************************************" << endl << endl;
                        ChangeLog << to_string(now->tm_year+1900) << '-'
                                   << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                   << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                   << setfill('0') << setw(2) << now->tm_min << ":"
                                   << setfill('0') << setw(2) << now->tm_sec << " - "
                                   << Matches[Suivant].first.GetChara() << " (" << Matches[Suivant].first.NomJoueur() << ") a vaincu "
                                   << Matches[Suivant].second.GetChara() << " (" << Matches[Suivant].second.NomJoueur() << ")" << endl << "*" << endl;
                    }
                if (Gagnant == Matches[Suivant].second.GetPlayer())

                    if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Matches[Suivant].second.GetNumPerso()))
                    {
                        J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                        Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                 << "*****************************************************************************************" << endl << endl;
                        ChangeLog << to_string(now->tm_year+1900) << '-'
                                   << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                   << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                   << setfill('0') << setw(2) << now->tm_min << ":"
                                   << setfill('0') << setw(2) << now->tm_sec << " - "
                                   << Matches[Suivant].second.GetChara() << " (" << Matches[Suivant].second.NomJoueur() << ") a vaincu "
                                   << Matches[Suivant].first.GetChara() << " (" << Matches[Suivant].first.NomJoueur() << ")" << endl << "*" << endl;
                    }
            }
            system("cls");
            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.SetRound(Matches[i].first.GetRound()-1);
                    Matches[i].second.SetRound(Matches[i].second.GetRound()-1);
                }
            }
        }
    }

    void M2v2 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        vector <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>> Matches;
        if (NbJoueur < 4) return;
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        ofstream ChangeLog ("Log.txt", ios_base::app);
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {
            unsigned J1;
            unsigned J2;
            unsigned J3;
            unsigned J4;
            J1 = rand() % NbJoueur + 1;
            J2 = rand() % NbJoueur + 1;
            J3 = rand() % NbJoueur + 1;
            J4 = rand() % NbJoueur + 1;
            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1);
            }
            while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4)
            {
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J3)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J4)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J1)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J1)
                    J4 = rand() % NbJoueur + 1;
                while (J3 == J4)
                    J3 = rand() % NbJoueur + 1;
            }
            unsigned P1 = rand() %  MPerso[J1 - 1].size();
            unsigned P2 = rand() %  MPerso[J2 - 1].size();
            unsigned P3 = rand() %  MPerso[J3 - 1].size();
            unsigned P4 = rand() %  MPerso[J4 - 1].size();
            pair <CCharacter, CCharacter> Team1;
            pair <CCharacter, CCharacter> Team2;
            pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>> Match;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second = J[i];
                    break;
                }
            }
            Match.first = Team1;
            Match.second = Team2;
            if (!Listing)
            {
                cout << endl;
                cout << "Match #" << Match.first.first.GetRound() << " (2v2) :" << endl;
                cout << Match.first.first.GetJoueur() << " " << Match.first.first.GetChara() << endl
                     << Match.first.second.GetJoueur() << " " << Match.first.second.GetChara() << endl
                     << "VS" << endl
                     << Match.second.first.GetJoueur() << " " << Match.second.first.GetChara() << endl
                     << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                Resultat << "Match #" << Match.first.first.GetRound() << " (2v2) :" << endl << endl;
                Resultat << Match.first.first.NomJoueur() << " " << Match.first.first.GetChara() << endl
                         << Match.first.second.NomJoueur() << " " << Match.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.NomJoueur() << " " << Match.second.first.GetChara() << endl
                         << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                unsigned Gagnant;
                cout << "Equipe gagnante : ";
                bool doesJ1 = false;
                cin >> Gagnant;
                switch (Gagnant)
                {
                    case 1 :
                    {
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team1.first.GetPlayer() && J[i].GetNumPerso() == Team1.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }

                            if ((J[i].GetPlayer() == Team1.second.GetPlayer() && J[i].GetNumPerso() == Team1.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                                ChangeLog << to_string(now->tm_year+1900) << '-'
                                           << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                           << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                           << setfill('0') << setw(2) << now->tm_min << ":"
                                           << setfill('0') << setw(2) << now->tm_sec << " - "
                                           << "L'équipe "
                                           << Match.first.first.GetChara() << " (" << Match.first.first.NomJoueur() << ") et "
                                           << Match.first.second.GetChara() << " (" << Match.first.second.NomJoueur() << ") ont vaincu l'équipe "
                                           << Match.second.first.GetChara() << " (" << Match.second.first.NomJoueur() << ") et "
                                           << Match.second.second.GetChara() << " (" << Match.second.second.NomJoueur() << ")" << endl << "*" << endl;
                            }
                        }
                        break;
                    }
                    case 2 :
                    {
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team2.first.GetPlayer() && J[i].GetNumPerso() == Team2.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team2.second.GetPlayer() && J[i].GetNumPerso() == Team2.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                                ChangeLog << to_string(now->tm_year+1900) << '-'
                                           << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                           << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                           << setfill('0') << setw(2) << now->tm_min << ":"
                                           << setfill('0') << setw(2) << now->tm_sec << " - "
                                           << "L'équipe "
                                           << Match.second.first.GetChara() << " (" << Match.second.first.NomJoueur() << ") et "
                                           << Match.second.second.GetChara() << " (" << Match.second.second.NomJoueur() << ") ont vaincu l'équipe "
                                           << Match.first.first.GetChara() << " (" << Match.first.first.NomJoueur() << ") et "
                                           << Match.first.second.GetChara() << " (" << Match.first.second.NomJoueur() << ")" << endl << "*" << endl;
                            }
                        }

                        break;
                    }
                    default :
                        break;
                }
            }
            else
            {
                Matches.push_back(Match);
            }
        }
        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.first.GetRound() << " : ";
                cout << Matches[i].first.first.NomJoueur() << " et " << Matches[i].first.second.NomJoueur() << " VS "
                     << Matches[i].second.first.NomJoueur() << " et "<<  Matches[i].second.second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            cout << endl;
            cout << "Match #" << Matches[Suivant].first.first.GetRound() << " : " << endl;
            cout << Matches[Suivant].first.first.GetJoueur() << " " << Matches[Suivant].first.first.GetChara() << endl
                 << Matches[Suivant].first.second.GetJoueur() << " " << Matches[Suivant].first.second.GetChara() << endl
                 << "VS" << endl
                 << Matches[Suivant].second.first.GetJoueur() << " " << Matches[Suivant].second.first.GetChara() << endl
                 << Matches[Suivant].second.second.GetJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;
            Resultat << Matches[Suivant].first.first.NomJoueur() << " " << Matches[Suivant].first.first.GetChara() << endl
                     << Matches[Suivant].first.second.NomJoueur() << " " << Matches[Suivant].first.second.GetChara() << endl
                     << "VS" << endl
                     << Matches[Suivant].second.first.NomJoueur() << " " << Matches[Suivant].second.first.GetChara() << endl
                     << Matches[Suivant].second.second.NomJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Equipe gagnante : ";
            cin >> Gagnant;
            Resultat << "Vainqueurs : ";
            bool last = false;
            switch (Gagnant)
            {
                case 1 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((!last) ? " & " : "\n") << ((last) ? "\n" : "");
                            last = true;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((!last) ? " & " : "\n") << ((last) ? "\n" : "");
                            last = true;
                            ChangeLog << to_string(now->tm_year+1900) << '-'
                                       << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                       << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                       << setfill('0') << setw(2) << now->tm_min << ":"
                                       << setfill('0') << setw(2) << now->tm_sec << " - "
                                       << "Léquipe "
                                       << Matches[Suivant].first.first.GetChara() << " (" << Matches[Suivant].first.first.NomJoueur() << ") et "
                                       << Matches[Suivant].first.second.GetChara() << " (" << Matches[Suivant].first.second.NomJoueur() << ") ont vaincu léquipe "
                                       << Matches[Suivant].second.first.GetChara() << " (" << Matches[Suivant].second.first.NomJoueur() << ") et "
                                       << Matches[Suivant].second.second.GetChara() << " (" << Matches[Suivant].second.second.NomJoueur() << ")" << endl << "*" << endl;
                        }
                    }
                    break;
                case 2 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((!last) ? " & " : "\n") << ((last) ? "\n" : "");
                            last = true;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((!last) ? " & " : "\n") << ((last) ? "\n" : "");
                            last = true;
                            ChangeLog << to_string(now->tm_year+1900) << '-'
                                       << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                       << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                       << setfill('0') << setw(2) << now->tm_min << ":"
                                       << setfill('0') << setw(2) << now->tm_sec << " - "
                                       << "L'équipe "
                                       << Matches[Suivant].second.first.GetChara() << " (" << Matches[Suivant].second.first.NomJoueur() << ") et "
                                       << Matches[Suivant].second.second.GetChara() << " (" << Matches[Suivant].second.second.NomJoueur() << ") ont vaincu l'équipe "
                                       << Matches[Suivant].first.first.GetChara() << " (" << Matches[Suivant].first.first.NomJoueur() << ") et "
                                       << Matches[Suivant].first.second.GetChara() << " (" << Matches[Suivant].first.second.NomJoueur() << ")" << endl << "*" << endl;
                        }
                    }
                    break;
                default :
                    break;
            }

            Resultat << "*****************************************************************************************" << endl << endl;

            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.first.SetRound(Matches[i].first.first.GetRound()-1);
                    Matches[i].second.first.SetRound(Matches[i].second.first.GetRound()-1);
                    Matches[i].first.second.SetRound(Matches[i].first.second.GetRound()-1);
                    Matches[i].second.second.SetRound(Matches[i].second.second.GetRound()-1);
                }
            }

        }
    }

    void M3v3 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        vector <pair <pair <pair <CCharacter, CCharacter>,CCharacter>,pair <pair <CCharacter, CCharacter>,CCharacter>>> Matches;
        if (NbJoueur < 6) return;
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        ofstream ChangeLog ("Log.txt", ios_base::app);
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {
            unsigned J1;
            unsigned J2;
            unsigned J3;
            unsigned J4;
            unsigned J5;
            unsigned J6;
            J1 = rand() % NbJoueur + 1;
            J2 = rand() % NbJoueur + 1;
            J3 = rand() % NbJoueur + 1;
            J4 = rand() % NbJoueur + 1;
            J5 = rand() % NbJoueur + 1;
            J6 = rand() % NbJoueur + 1;
            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1);
            }
            while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5 || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6)
            {
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J3)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J4)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J1)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J1)
                    J4 = rand() % NbJoueur + 1;
                while (J3 == J4)
                    J3 = rand() % NbJoueur + 1;
                while (J2 == J5)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J6)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J5)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J6)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J5)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J6)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J6)
                    J5 = rand() % NbJoueur + 1;
                while (J1 == J6)
                    J6 = rand() % NbJoueur + 1;
                while (J5 == J1)
                    J5 = rand() % NbJoueur + 1;
            }
            unsigned P1 = rand() %  MPerso[J1 - 1].size();
            unsigned P2 = rand() %  MPerso[J2 - 1].size();
            unsigned P3 = rand() %  MPerso[J3 - 1].size();
            unsigned P4 = rand() %  MPerso[J4 - 1].size();
            unsigned P5 = rand() %  MPerso[J5 - 1].size();
            unsigned P6 = rand() %  MPerso[J6 - 1].size();
            pair <pair <CCharacter, CCharacter>,CCharacter> Team1;
            pair <pair <CCharacter, CCharacter>,CCharacter> Team2;
            pair <pair <pair <CCharacter, CCharacter>,CCharacter>,pair <pair <CCharacter, CCharacter>,CCharacter>> Match;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second = J[i];
                    break;
                }
            }
            Match.first = Team1;
            Match.second = Team2;
            if (!Listing)
            {
                cout << endl;
                cout << "Match #" << Match.first.first.first.GetRound() << " (3v3) :" << endl;
                cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                     << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                     << Match.first.second.GetJoueur() << " " << Match.first.second.GetChara() << endl
                     << "VS" << endl
                     << Match.second.first.first.GetJoueur() << " " << Match.second.first.first.GetChara() << endl
                     << Match.second.first.second.GetJoueur() << " " << Match.second.first.second.GetChara() << endl
                     << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                Resultat << "Match #" << Match.first.first.first.GetRound() << " (3v3) :" << endl << endl;
                Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << Match.first.second.NomJoueur() << " " << Match.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.first.NomJoueur() << " " << Match.second.first.first.GetChara() << endl
                         << Match.second.first.second.NomJoueur() << " " << Match.second.first.second.GetChara() << endl
                         << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                unsigned Gagnant;
                cout << "Equipe gagnante : ";
                bool doesJ1 = false;
                bool doesJ2 = false;
                cin >> Gagnant;
                switch (Gagnant)
                {
                    case 1 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team1.first.first.GetPlayer() && J[i].GetNumPerso() == Team1.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team1.first.second.GetPlayer() && J[i].GetNumPerso() == Team1.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team1.second.GetPlayer() && J[i].GetNumPerso() == Team1.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                        }
                        ChangeLog << to_string(now->tm_year+1900) << '-'
                                   << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                   << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                   << setfill('0') << setw(2) << now->tm_min << ":"
                                   << setfill('0') << setw(2) << now->tm_sec << " - "
                                   << "L'équipe "
                                   << Match.first.first.first.GetChara() << " (" << Match.first.first.first.NomJoueur() << "), "
                                   << Match.first.first.second.GetChara() << " (" << Match.first.first.second.NomJoueur() << ") et "
                                   << Match.first.second.GetChara() << " (" << Match.first.second.NomJoueur() << ") ont vaincu l'équipe "
                                   << Match.second.first.first.GetChara() << " (" << Match.second.first.first.NomJoueur() << "), "
                                   << Match.second.first.second.GetChara() << " (" << Match.second.first.second.NomJoueur() << ") et "
                                   << Match.second.second.GetChara() << " (" << Match.second.second.NomJoueur() << ")" << endl << "*" << endl;
                        break;
                    case 2 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team2.first.first.GetPlayer() && J[i].GetNumPerso() == Team2.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team2.first.second.GetPlayer() && J[i].GetNumPerso() == Team2.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team2.second.GetPlayer() && J[i].GetNumPerso() == Team2.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ2) ? "\n\n" : "");
                                if (doesJ2)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                        }
                        ChangeLog << to_string(now->tm_year+1900) << '-'
                                   << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                                   << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                                   << setfill('0') << setw(2) << now->tm_min << ":"
                                   << setfill('0') << setw(2) << now->tm_sec << " - "
                                   << "L'équipe "
                                   << Match.second.first.first.GetChara() << " (" << Match.second.first.first.NomJoueur() << "), "
                                   << Match.second.first.second.GetChara() << " (" << Match.second.first.second.NomJoueur() << ") et "
                                   << Match.second.second.GetChara() << " (" << Match.second.second.NomJoueur() << ") ont vaincu l'équipe "
                                   << Match.first.first.first.GetChara() << " (" << Match.first.first.first.NomJoueur() << "), "
                                   << Match.first.first.second.GetChara() << " (" << Match.first.first.second.NomJoueur() << ") et "
                                   << Match.first.second.GetChara() << " (" << Match.first.second.NomJoueur() << ")" << endl << "*" << endl;
                        break;
                    default :
                        break;
                }
            }
            else
                Matches.push_back(Match);
        }
        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.first.first.GetRound() << " : ";
                cout << Matches[i].first.first.first.NomJoueur() << ", " << Matches[i].first.first.second.NomJoueur() << " & " << endl << Matches[i].first.second.NomJoueur() << " VS "
                     << Matches[i].second.first.first.NomJoueur() << ", "<<  Matches[i].second.first.second.NomJoueur() << " & " << endl << Matches[i].second.second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            cout  << endl;
            cout  << "Match #" << Matches[Suivant].first.first.first.GetRound() << " : " << endl;
            cout  << Matches[Suivant].first.first.first.GetJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                  << Matches[Suivant].first.first.second.GetJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                  << Matches[Suivant].first.second.GetJoueur() << " " << Matches[Suivant].first.second.GetChara() << endl
                  << "VS" << endl
                  << Matches[Suivant].second.first.first.GetJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                  << Matches[Suivant].second.first.second.GetJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                  << Matches[Suivant].second.second.GetJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;

            Resultat  << Matches[Suivant].first.first.first.NomJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                      << Matches[Suivant].first.first.second.NomJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                      << Matches[Suivant].first.second.NomJoueur() << " " << Matches[Suivant].first.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].second.first.first.NomJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                      << Matches[Suivant].second.first.second.NomJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                      << Matches[Suivant].second.second.NomJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Equipe gagnante : ";
            cin >> Gagnant;
            Resultat << "Vainqueurs : ";
            unsigned TimesWrite (1);
            switch (Gagnant)
            {
                case 1 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    ChangeLog << to_string(now->tm_year+1900) << '-'
                               << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                               << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                               << setfill('0') << setw(2) << now->tm_min << ":"
                               << setfill('0') << setw(2) << now->tm_sec << " - "
                               << "L'équipe "
                               << Matches[Suivant].first.first.first.GetChara() << " (" << Matches[Suivant].first.first.first.NomJoueur() << "), "
                               << Matches[Suivant].first.first.second.GetChara() << " (" << Matches[Suivant].first.first.second.NomJoueur() << ") et "
                               << Matches[Suivant].first.second.GetChara() << " (" << Matches[Suivant].first.second.NomJoueur() << ") ont vaincu l'équipe "
                               << Matches[Suivant].second.first.first.GetChara() << " (" << Matches[Suivant].second.first.first.NomJoueur() << "), "
                               << Matches[Suivant].second.first.second.GetChara() << " (" << Matches[Suivant].second.first.second.NomJoueur() << ") et "
                               << Matches[Suivant].second.second.GetChara() << " (" << Matches[Suivant].second.second.NomJoueur() << ")" << endl << "*" << endl;
                    break;
                case 2 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? ", " : (TimesWrite == 2) ? " & " : "\n") << ((TimesWrite == 3) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    ChangeLog << to_string(now->tm_year+1900) << '-'
                               << ((to_string(now->tm_mon+1).size() == 1) ? '0' + to_string(now->tm_mon+1) : to_string(now->tm_mon+1))
                               << '-' << to_string(now->tm_mday) << " " << setfill('0') << setw(2) << now->tm_hour << ":"
                               << setfill('0') << setw(2) << now->tm_min << ":"
                               << setfill('0') << setw(2) << now->tm_sec << " - "
                               << "L'équipe "
                               << Matches[Suivant].second.first.first.GetChara() << " (" << Matches[Suivant].second.first.first.NomJoueur() << "), "
                               << Matches[Suivant].second.first.second.GetChara() << " (" << Matches[Suivant].second.first.second.NomJoueur() << ") et "
                               << Matches[Suivant].second.second.GetChara() << " (" << Matches[Suivant].second.second.NomJoueur() << ") ont vaincu l'équipe "
                               << Matches[Suivant].first.first.first.GetChara() << " (" << Matches[Suivant].first.first.first.NomJoueur() << "), "
                               << Matches[Suivant].first.first.second.GetChara() << " (" << Matches[Suivant].first.first.second.NomJoueur() << ") et "
                               << Matches[Suivant].first.second.GetChara() << " (" << Matches[Suivant].first.second.NomJoueur() << ")" << endl << "*" << endl;
                    break;
                default :
                    break;
            }

            Resultat << "*****************************************************************************************" << endl << endl;

            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.first.first.SetRound(Matches[i].first.first.first.GetRound()-1);
                    Matches[i].first.first.second.SetRound(Matches[i].first.first.second.GetRound()-1);
                    Matches[i].first.second.SetRound(Matches[i].first.second.GetRound()-1);
                    Matches[i].second.first.first.SetRound(Matches[i].second.first.first.GetRound()-1);
                    Matches[i].second.first.second.SetRound(Matches[i].second.first.second.GetRound()-1);
                    Matches[i].second.second.SetRound(Matches[i].second.second.GetRound()-1);
                }
            }

        }
    }

    void M4v4 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        vector <pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>>> Matches;
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        if (NbJoueur < 8) return;
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {
            unsigned J1;
            unsigned J2;
            unsigned J3;
            unsigned J4;
            unsigned J5;
            unsigned J6;
            unsigned J7;
            unsigned J8;
            J1 = rand() % NbJoueur + 1;
            J2 = rand() % NbJoueur + 1;
            J3 = rand() % NbJoueur + 1;
            J4 = rand() % NbJoueur + 1;
            J5 = rand() % NbJoueur + 1;
            J6 = rand() % NbJoueur + 1;
            J7 = rand() % NbJoueur + 1;
            J8 = rand() % NbJoueur + 1;
            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1);
            }
            while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5 || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6 || J2 == J7 || J2 == J8 || J3 == J7 || J3 == J8 || J4 == J7 || J4 == J8 || J5 == J7 || J5 == J8 || J6 == J7 || J6 == J8 || J7 == J8 || J1 == J7 || J1 == J8)
            {
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J3)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J4)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J1)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J1)
                    J4 = rand() % NbJoueur + 1;
                while (J3 == J4)
                    J3 = rand() % NbJoueur + 1;
                while (J2 == J5)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J6)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J5)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J6)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J5)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J6)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J6)
                    J5 = rand() % NbJoueur + 1;
                while (J1 == J6)
                    J6 = rand() % NbJoueur + 1;
                while (J5 == J1)
                    J5 = rand() % NbJoueur + 1;
                while (J2 == J7)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J8)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J7)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J8)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J7)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J8)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J7)
                    J5 = rand() % NbJoueur + 1;
                while (J5 == J8)
                    J5 = rand() % NbJoueur + 1;
                while (J6 == J7)
                    J6 = rand() % NbJoueur + 1;
                while (J6 == J8)
                    J6 = rand() % NbJoueur + 1;
                while (J1 == J7)
                    J7 = rand() % NbJoueur + 1;
                while (J1 == J8)
                    J8 = rand() % NbJoueur + 1;
                while (J7 == J8)
                    J8 = rand() % NbJoueur + 1;
            }
            unsigned P1 = rand() %  MPerso[J1 - 1].size();
            unsigned P2 = rand() %  MPerso[J2 - 1].size();
            unsigned P3 = rand() %  MPerso[J3 - 1].size();
            unsigned P4 = rand() %  MPerso[J4 - 1].size();
            unsigned P5 = rand() %  MPerso[J5 - 1].size();
            unsigned P6 = rand() %  MPerso[J6 - 1].size();
            unsigned P7 = rand() %  MPerso[J7 - 1].size();
            unsigned P8 = rand() %  MPerso[J8 - 1].size();
            pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>> Team1;
            pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>> Team2;
            pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>> Match;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J7 && J[i].GetNumPerso() == P7))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J8 && J[i].GetNumPerso() == P8))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first.first = J[i];
                    break;
                }
            }
            Match.first = Team1;
            Match.second = Team2;
            if (!Listing)
            {
                cout << endl;
                cout << "Match #" << Match.first.first.first.GetRound() << " (4v4) :" << endl;
                cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                     << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                     << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                     << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                     << "VS" << endl
                     << Match.second.first.first.GetJoueur() << " " << Match.second.first.first.GetChara() << endl
                     << Match.second.first.second.GetJoueur() << " " << Match.second.first.second.GetChara() << endl
                     << Match.second.second.first.GetJoueur() << " " << Match.second.second.first.GetChara() << endl
                     << Match.second.second.second.GetJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;

                Resultat << "Match #" << Match.first.first.first.GetRound() << " (4v4) :" << endl << endl;
                Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.first.NomJoueur() << " " << Match.second.first.first.GetChara() << endl
                         << Match.second.first.second.NomJoueur() << " " << Match.second.first.second.GetChara() << endl
                         << Match.second.second.first.NomJoueur() << " " << Match.second.second.first.GetChara() << endl
                         << Match.second.second.second.NomJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;
                unsigned Gagnant;
                cout << "Equipe gagnante : ";
                bool doesJ1 = false;
                bool doesJ2 = false;
                bool doesJ3 = false;
                cin >> Gagnant;
                switch (Gagnant)
                {
                    case 1 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team1.first.first.GetPlayer() && J[i].GetNumPerso() == Team1.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team1.first.second.GetPlayer() && J[i].GetNumPerso() == Team1.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team1.second.first.GetPlayer() && J[i].GetNumPerso() == Team1.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team1.second.second.GetPlayer() && J[i].GetNumPerso() == Team1.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                        }
                        break;
                    case 2 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team2.first.first.GetPlayer() && J[i].GetNumPerso() == Team2.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team2.first.second.GetPlayer() && J[i].GetNumPerso() == Team2.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team2.second.first.GetPlayer() && J[i].GetNumPerso() == Team2.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                            if ((J[i].GetPlayer() == Team2.second.second.GetPlayer() && J[i].GetNumPerso() == Team2.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? ((doesJ2) ? ((doesJ3) ? J[i].NomJoueur() : J[i].NomJoueur() + " & ") : J[i].NomJoueur() + ", ") : "Vainqueurs : " + J[i].NomJoueur() + ", ") << ((doesJ3) ? "\n\n" : "");
                                if (doesJ3)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ3 && doesJ2 && doesJ1) ? doesJ3 = true : doesJ3);
                                ((!doesJ2 && doesJ1) ? doesJ2 = true : doesJ2);
                                ((!doesJ1) ? doesJ1 = true : doesJ1);

                            }
                        }
                        break;
                    default :
                        break;
                }
            }
            else
                Matches.push_back(Match);
        }
        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.first.first.GetRound() << " : ";
                cout << Matches[i].first.first.first.NomJoueur() << ", " << Matches[i].first.first.second.NomJoueur() << ", " << endl
                     << Matches[i].first.second.first.NomJoueur() << " et " << Matches[i].first.second.second.NomJoueur() << " VS "
                     << Matches[i].second.first.first.NomJoueur() << ", " <<  Matches[i].second.first.second.NomJoueur() << ", " << endl
                     << Matches[i].second.second.first.NomJoueur() << " et " << Matches[i].second.second.second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            cout  << endl;
            cout  << "Match #" << Matches[Suivant].first.first.first.GetRound() << " : " << endl;
            cout  << Matches[Suivant].first.first.first.GetJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                  << Matches[Suivant].first.first.second.GetJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                  << Matches[Suivant].first.second.first.GetJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                  << Matches[Suivant].first.second.second.GetJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                  << "VS" << endl
                  << Matches[Suivant].second.first.first.GetJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                  << Matches[Suivant].second.first.second.GetJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                  << Matches[Suivant].second.second.first.GetJoueur() << " " << Matches[Suivant].second.second.first.GetChara() << endl
                  << Matches[Suivant].second.second.second.GetJoueur() << " " << Matches[Suivant].second.second.second.GetChara() << endl << endl;

            Resultat  << Matches[Suivant].first.first.first.NomJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                      << Matches[Suivant].first.first.second.NomJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                      << Matches[Suivant].first.second.first.NomJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                      << Matches[Suivant].first.second.second.NomJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].second.first.first.NomJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                      << Matches[Suivant].second.first.second.NomJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                      << Matches[Suivant].second.second.first.NomJoueur() << " " << Matches[Suivant].second.second.first.GetChara() << endl
                      << Matches[Suivant].second.second.second.NomJoueur() << " " << Matches[Suivant].second.second.second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Equipe gagnante : ";
            cin >> Gagnant;
            Resultat << "Vainqueurs : ";
            unsigned TimesWrite (1);
            switch (Gagnant)
            {
                case 1 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 2 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1 || TimesWrite == 2) ? ", " : (TimesWrite == 3) ? " & " : "\n") << ((TimesWrite == 4) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                default :
                    break;
            }

            Resultat << "*****************************************************************************************" << endl << endl;

            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.first.first.SetRound(Matches[i].first.first.first.GetRound()-1);
                    Matches[i].first.first.second.SetRound(Matches[i].first.first.second.GetRound()-1);
                    Matches[i].first.second.first.SetRound(Matches[i].first.second.first.GetRound()-1);
                    Matches[i].first.second.second.SetRound(Matches[i].first.second.second.GetRound()-1);
                    Matches[i].second.first.first.SetRound(Matches[i].second.first.first.GetRound()-1);
                    Matches[i].second.first.second.SetRound(Matches[i].second.first.second.GetRound()-1);
                    Matches[i].second.second.first.SetRound(Matches[i].second.second.first.GetRound()-1);
                    Matches[i].second.second.second.SetRound(Matches[i].second.second.second.GetRound()-1);
                }
            }

        }

    }

    void M2v2x3 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        vector <pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <CCharacter, CCharacter>>> Matches;
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        if (NbJoueur < 6) return;
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {
            unsigned J1;
            unsigned J2;
            unsigned J3;
            unsigned J4;
            unsigned J5;
            unsigned J6;
            J1 = rand() % NbJoueur + 1;
            J2 = rand() % NbJoueur + 1;
            J3 = rand() % NbJoueur + 1;
            J4 = rand() % NbJoueur + 1;
            J5 = rand() % NbJoueur + 1;
            J6 = rand() % NbJoueur + 1;
            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1);
            }
            while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5 || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6)
            {
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J3)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J4)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J1)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J1)
                    J4 = rand() % NbJoueur + 1;
                while (J3 == J4)
                    J3 = rand() % NbJoueur + 1;
                while (J2 == J5)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J6)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J5)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J6)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J5)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J6)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J6)
                    J5 = rand() % NbJoueur + 1;
                while (J1 == J6)
                    J6 = rand() % NbJoueur + 1;
                while (J5 == J1)
                    J5 = rand() % NbJoueur + 1;
            }
            unsigned P1 = rand() %  MPerso[J1 - 1].size();
            unsigned P2 = rand() %  MPerso[J2 - 1].size();
            unsigned P3 = rand() %  MPerso[J3 - 1].size();
            unsigned P4 = rand() %  MPerso[J4 - 1].size();
            unsigned P5 = rand() %  MPerso[J5 - 1].size();
            unsigned P6 = rand() %  MPerso[J6 - 1].size();
            pair <CCharacter, CCharacter> Team1;
            pair <CCharacter, CCharacter> Team2;
            pair <CCharacter, CCharacter> Team3;
            pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <CCharacter, CCharacter>> Match;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team3.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team3.second = J[i];
                    break;
                }
            }
            Match.first.first = Team1;
            Match.first.second = Team2;
            Match.second = Team3;
            if (!Listing)
            {
                cout << endl;
                cout << "Match #" << Match.first.first.first.GetRound() << " (2v2v2) :" << endl;
                cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                     << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                     << "VS" << endl
                     << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                     << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                     << "VS" << endl
                     << Match.second.first.GetJoueur() << " " << Match.second.first.GetChara() << endl
                     << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                Resultat << "Match #" << Match.first.first.first.GetRound() << " (2v2v2) :" << endl << endl;
                Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.NomJoueur() << " " << Match.second.first.GetChara() << endl
                         << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                unsigned Gagnant;
                cout << "Equipe gagnante : ";
                bool doesJ1 = false;
                cin >> Gagnant;
                switch (Gagnant)
                {
                    case 1 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team1.first.GetPlayer() && J[i].GetNumPerso() == Team1.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team1.second.GetPlayer() && J[i].GetNumPerso() == Team1.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    case 2 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team2.first.GetPlayer() && J[i].GetNumPerso() == Team2.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team2.second.GetPlayer() && J[i].GetNumPerso() == Team2.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    case 3 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team3.first.GetPlayer() && J[i].GetNumPerso() == Team3.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team3.second.GetPlayer() && J[i].GetNumPerso() == Team3.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    default :
                        break;
                }
            }
            else
                Matches.push_back(Match);

        }
        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.first.first.GetRound() << " : ";
                cout << Matches[i].first.first.first.NomJoueur() << " et " << Matches[i].first.first.second.NomJoueur() << " VS " << endl
                     << Matches[i].first.second.first.NomJoueur() << " et " << Matches[i].first.second.second.NomJoueur() << " VS " << endl
                     << Matches[i].second.first.NomJoueur() << " et " <<  Matches[i].second.second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            cout  << endl;
            cout << "Match #" << Matches[Suivant].first.first.first.GetRound() << ":" << endl;
            cout << Matches[Suivant].first.first.first.GetJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                 << Matches[Suivant].first.first.second.GetJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                 << "VS" << endl
                 << Matches[Suivant].first.second.first.GetJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                 << Matches[Suivant].first.second.second.GetJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                 << "VS" << endl
                 << Matches[Suivant].second.first.GetJoueur() << " " << Matches[Suivant].second.first.GetChara() <<endl
                 << Matches[Suivant].second.second.GetJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;

            Resultat  << Matches[Suivant].first.first.first.NomJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                      << Matches[Suivant].first.first.second.NomJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].first.second.first.NomJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                      << Matches[Suivant].first.second.second.NomJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].second.first.NomJoueur() << " " << Matches[Suivant].second.first.GetChara() <<endl
                      << Matches[Suivant].second.second.NomJoueur() << " " << Matches[Suivant].second.second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Equipe gagnante : ";
            cin >> Gagnant;
            Resultat << "Vainqueurs : ";
            unsigned TimesWrite (1);
            switch (Gagnant)
            {
                case 1 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 2 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 3 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                default :
                    break;
            }

            Resultat << "*****************************************************************************************" << endl << endl;

            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.first.first.SetRound(Matches[i].first.first.first.GetRound()-1);
                    Matches[i].first.first.second.SetRound(Matches[i].first.first.second.GetRound()-1);
                    Matches[i].first.second.first.SetRound(Matches[i].first.second.first.GetRound()-1);
                    Matches[i].first.second.second.SetRound(Matches[i].first.second.second.GetRound()-1);
                    Matches[i].second.first.SetRound(Matches[i].second.first.GetRound()-1);
                    Matches[i].second.second.SetRound(Matches[i].second.second.GetRound()-1);
                }
            }
        }

    }

    void M2v2x4 (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso, bool Listing)
    {
        vector <pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>>> Matches;
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        if (NbJoueur < 8) return;
        for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
        {
            unsigned J1;
            unsigned J2;
            unsigned J3;
            unsigned J4;
            unsigned J5;
            unsigned J6;
            unsigned J7;
            unsigned J8;
            J1 = rand() % NbJoueur + 1;
            J2 = rand() % NbJoueur + 1;
            J3 = rand() % NbJoueur + 1;
            J4 = rand() % NbJoueur + 1;
            J5 = rand() % NbJoueur + 1;
            J6 = rand() % NbJoueur + 1;
            J7 = rand() % NbJoueur + 1;
            J8 = rand() % NbJoueur + 1;
            for (unsigned i (0); i < J.size(); ++i)
            {
                J[i].SetRound(J[i].GetRound()+1);
            }
            while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5 || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6 || J2 == J7 || J2 == J8 || J3 == J7 || J3 == J8 || J4 == J7 || J4 == J8 || J5 == J7 || J5 == J8 || J6 == J7 || J6 == J8 || J7 == J8 || J1 == J7 || J1 == J8)
            {
                while (J2 == J1)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J3)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J4)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J1)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J1)
                    J4 = rand() % NbJoueur + 1;
                while (J3 == J4)
                    J3 = rand() % NbJoueur + 1;
                while (J2 == J5)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J6)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J5)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J6)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J5)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J6)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J6)
                    J5 = rand() % NbJoueur + 1;
                while (J1 == J6)
                    J6 = rand() % NbJoueur + 1;
                while (J5 == J1)
                    J5 = rand() % NbJoueur + 1;
                while (J2 == J7)
                    J2 = rand() % NbJoueur + 1;
                while (J2 == J8)
                    J2 = rand() % NbJoueur + 1;
                while (J3 == J7)
                    J3 = rand() % NbJoueur + 1;
                while (J3 == J8)
                    J3 = rand() % NbJoueur + 1;
                while (J4 == J7)
                    J4 = rand() % NbJoueur + 1;
                while (J4 == J8)
                    J4 = rand() % NbJoueur + 1;
                while (J5 == J7)
                    J5 = rand() % NbJoueur + 1;
                while (J5 == J8)
                    J5 = rand() % NbJoueur + 1;
                while (J6 == J7)
                    J6 = rand() % NbJoueur + 1;
                while (J6 == J8)
                    J6 = rand() % NbJoueur + 1;
                while (J1 == J7)
                    J7 = rand() % NbJoueur + 1;
                while (J1 == J8)
                    J8 = rand() % NbJoueur + 1;
                while (J7 == J8)
                    J8 = rand() % NbJoueur + 1;
            }
            unsigned P1 = rand() %  MPerso[J1 - 1].size();
            unsigned P2 = rand() %  MPerso[J2 - 1].size();
            unsigned P3 = rand() %  MPerso[J3 - 1].size();
            unsigned P4 = rand() %  MPerso[J4 - 1].size();
            unsigned P5 = rand() %  MPerso[J5 - 1].size();
            unsigned P6 = rand() %  MPerso[J6 - 1].size();
            unsigned P7 = rand() %  MPerso[J7 - 1].size();
            unsigned P8 = rand() %  MPerso[J8 - 1].size();
            pair <CCharacter, CCharacter> Team1;
            pair <CCharacter, CCharacter> Team2;
            pair <CCharacter, CCharacter> Team3;
            pair <CCharacter, CCharacter> Team4;
            pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>> Match;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team1.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team2.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team3.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team3.second = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J7 && J[i].GetNumPerso() == P7))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team4.first = J[i];
                    break;
                }
            }
            for (unsigned i (0); i < J.size(); ++i)
            {
                if ((J[i].GetPlayer() == J8 && J[i].GetNumPerso() == P8))
                {
                    J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                    Team4.second = J[i];
                    break;
                }
            }
            Match.first.first = Team1;
            Match.first.second = Team2;
            Match.second.first = Team3;
            Match.second.second = Team4;
            if (!Listing)
            {
                cout << endl;
                cout << "Match #" << Match.first.first.first.GetRound() << " (2v2v2v2) :" << endl;
                cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                     << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                     << "VS" << endl
                     << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                     << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                     << "VS" << endl
                     << Match.second.first.first.GetJoueur() << " " << Match.second.first.first.GetChara() << endl
                     << Match.second.first.second.GetJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                     << "VS" << endl
                     << Match.second.second.first.GetJoueur() << " " << Match.second.second.first.GetChara() << endl
                     << Match.second.second.second.GetJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;

                Resultat << "Match #" << Match.first.first.first.GetRound() << " (2v2v2v2) :" << endl << endl;
                Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.first.NomJoueur() << " " << Match.second.first.first.GetChara() << endl
                         << Match.second.first.second.NomJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                         << "VS" << endl
                         << Match.second.second.first.NomJoueur() << " " << Match.second.second.first.GetChara() << endl
                         << Match.second.second.second.NomJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;
                unsigned Gagnant;
                cout << "Equipe gagnante : ";
                bool doesJ1 = false;
                cin >> Gagnant;
                switch (Gagnant)
                {
                    case 1 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team1.first.GetPlayer() && J[i].GetNumPerso() == Team1.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team1.second.GetPlayer() && J[i].GetNumPerso() == Team1.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    case 2 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team2.first.GetPlayer() && J[i].GetNumPerso() == Team2.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team2.second.GetPlayer() && J[i].GetNumPerso() == Team2.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    case 3 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team3.first.GetPlayer() && J[i].GetNumPerso() == Team3.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team3.second.GetPlayer() && J[i].GetNumPerso() == Team3.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    case 4 :
                        for (unsigned i (0); i < J.size(); ++i)
                        {
                            if ((J[i].GetPlayer() == Team4.first.GetPlayer() && J[i].GetNumPerso() == Team4.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                            if ((J[i].GetPlayer() == Team4.second.GetPlayer() && J[i].GetNumPerso() == Team4.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                                Resultat << ((doesJ1) ? J[i].NomJoueur() : "Vainqueurs : " + J[i].NomJoueur() + " & ") << ((doesJ1) ? "\n\n" : "");
                                if (doesJ1)
                                    Resultat << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                                ((!doesJ1) ? doesJ1 = true : doesJ1);
                            }
                        }
                        break;
                    default :
                        break;
                }
            }
            else
                Matches.push_back(Match);
        }
        while (Matches.size() > 0)
        {
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;
                cout << "Match #" << Matches[i].first.first.first.GetRound() << " : ";
                cout << Matches[i].first.first.first.NomJoueur() << " et " << Matches[i].first.first.second.NomJoueur() << " VS " << endl
                     << Matches[i].first.second.first.NomJoueur() << " et " << Matches[i].first.second.second.NomJoueur() << " VS " << endl
                     << Matches[i].second.first.first.NomJoueur() << " et "<<  Matches[i].second.first.second.NomJoueur() << " VS " << endl
                     << Matches[i].second.second.first.NomJoueur() << " et " << Matches[i].second.second.second.NomJoueur() << endl;
            }
            cout << endl;
            unsigned Suivant;
            cout << "Match suivant : ";
            cin >> Suivant;
            Suivant--;
            cout  << endl;
            cout  << "Match #" << Matches[Suivant].first.first.first.GetRound() << " : " << endl;
            cout  << Matches[Suivant].first.first.first.GetJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                  << Matches[Suivant].first.first.second.GetJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                  << "VS" << endl
                  << Matches[Suivant].first.second.first.GetJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                  << Matches[Suivant].first.second.second.GetJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                  << "VS" << endl
                  << Matches[Suivant].second.first.first.GetJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                  << Matches[Suivant].second.first.second.GetJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                  << "VS" << endl
                  << Matches[Suivant].second.second.first.GetJoueur() << " " << Matches[Suivant].second.second.first.GetChara() << endl
                  << Matches[Suivant].second.second.second.GetJoueur() << " " << Matches[Suivant].second.second.second.GetChara() << endl << endl;

            Resultat  << Matches[Suivant].first.first.first.NomJoueur() << " " << Matches[Suivant].first.first.first.GetChara() << endl
                      << Matches[Suivant].first.first.second.NomJoueur() << " " << Matches[Suivant].first.first.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].first.second.first.NomJoueur() << " " << Matches[Suivant].first.second.first.GetChara() << endl
                      << Matches[Suivant].first.second.second.NomJoueur() << " " << Matches[Suivant].first.second.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].second.first.first.NomJoueur() << " " << Matches[Suivant].second.first.first.GetChara() << endl
                      << Matches[Suivant].second.first.second.NomJoueur() << " " << Matches[Suivant].second.first.second.GetChara() << endl
                      << "VS" << endl
                      << Matches[Suivant].second.second.first.NomJoueur() << " " << Matches[Suivant].second.second.first.GetChara() << endl
                      << Matches[Suivant].second.second.second.NomJoueur() << " " << Matches[Suivant].second.second.second.GetChara() << endl << endl;
            unsigned Gagnant;
            cout << "Equipe gagnante : ";
            cin >> Gagnant;
            Resultat << "Vainqueurs : ";
            unsigned TimesWrite (1);
            switch (Gagnant)
            {
                case 1 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 2:
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].first.second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].first.second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 3 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.first.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.first.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                case 4 :
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.first.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.first.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                        if ((J[i].GetPlayer() == Matches[Suivant].second.second.second.GetPlayer() && J[i].GetNumPerso() == Matches[Suivant].second.second.second.GetNumPerso()))
                        {
                            J[i].SetNbVictoire(J[i].GetNbVictoire() + 1);
                            Resultat << J[i].NomJoueur() << ((TimesWrite == 1) ? " & " : "\n") << ((TimesWrite == 2) ? "\n" : "");
                            TimesWrite++;
                        }
                    }
                    break;
                default :
                    break;
            }

            Resultat << "*****************************************************************************************" << endl << endl;

            Matches.erase(Matches.begin()+(Suivant));
            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                if (i >= Suivant)
                {
                    Matches[i].first.first.first.SetRound(Matches[i].first.first.first.GetRound()-1);
                    Matches[i].first.first.second.SetRound(Matches[i].first.first.second.GetRound()-1);
                    Matches[i].first.second.first.SetRound(Matches[i].first.second.first.GetRound()-1);
                    Matches[i].first.second.second.SetRound(Matches[i].first.second.second.GetRound()-1);
                    Matches[i].second.first.first.SetRound(Matches[i].second.first.first.GetRound()-1);
                    Matches[i].second.first.second.SetRound(Matches[i].second.first.second.GetRound()-1);
                    Matches[i].second.second.first.SetRound(Matches[i].second.second.first.GetRound()-1);
                    Matches[i].second.second.second.SetRound(Matches[i].second.second.second.GetRound()-1);
                }
            }
        }

    }

    void FFA (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso)
    {
        vector <string> Vterrain;
        vector <string> VFD;
        if (NbJoueur >= 6 && NbJoueur <=7)
        {

            Vterrain.push_back("Battlefield");
            Vterrain.push_back("Big Battlefield");
            Vterrain.push_back("Final Destination");
            Vterrain.push_back("Mario Galaxy");
            Vterrain.push_back("Super Mario Maker");
            Vterrain.push_back("Peach's Castle (64)");
            Vterrain.push_back("Luigi's Mansion");
            Vterrain.push_back("Mario Circuit (Brawl)");
            Vterrain.push_back("Yoshi's Island");
            Vterrain.push_back("Kongo Jungle 64");
            Vterrain.push_back("Hyrule Castle (64)");
            Vterrain.push_back("Temple");
            Vterrain.push_back("Bridge of Eldin");
            Vterrain.push_back("Pyrosphere");
            Vterrain.push_back("Norfair");
            Vterrain.push_back("Dream Land (64)");
            Vterrain.push_back("Lylat Cruise");
            Vterrain.push_back("Pokemon Stadium 2");
            Vterrain.push_back("Onett");
            Vterrain.push_back("Castle Siege");
            Vterrain.push_back("Flat Zone X");
            Vterrain.push_back("Skyworld");
            Vterrain.push_back("Town and City");
            Vterrain.push_back("Smashville");
            Vterrain.push_back("Wii Fit Studio");
            Vterrain.push_back("Gaur Plain");
            Vterrain.push_back("Duck Hunt");
            Vterrain.push_back("Windy Hill Zone");
            Vterrain.push_back("Pac-Land");
            Vterrain.push_back("Suzaku Castle");
            Vterrain.push_back("Midgar");
            Vterrain.push_back("Umbra Clock Tower");
            VFD.push_back("Battlefield FD");
            VFD.push_back("Big Battlefield FD");
            VFD.push_back("Mario Galaxy FD");
            VFD.push_back("Mushroom Kingdom U FD");
            VFD.push_back("Super Mario Maker FD");
            VFD.push_back("Peach's Castle (64) FD");
            VFD.push_back("Delfino Plaza FD");
            VFD.push_back("Luigi's Mansion FD");
            VFD.push_back("Mario Circuit (Brawl) FD");
            VFD.push_back("Yoshi's Island FD");
            VFD.push_back("Jungle Hijinxs FD");
            VFD.push_back("Kongo Jungle 64 FD");
            VFD.push_back("75m FD");
            VFD.push_back("Hyrule Castle (64) FD");
            VFD.push_back("Temple FD");
            VFD.push_back("Bridge of Eldin FD");
            VFD.push_back("Pirate Ship FD");
            VFD.push_back("Pyrosphere FD");
            VFD.push_back("Norfair FD");
            VFD.push_back("The Great Cave Offensive FD");
            VFD.push_back("Dream Land (64) FD");
            VFD.push_back("Halberd FD");
            VFD.push_back("Orbital Gate Assault FD");
            VFD.push_back("Lylat Cruise FD");
            VFD.push_back("Kalos Pokémon League FD");
            VFD.push_back("Pokémon Stadium 2 FD");
            VFD.push_back("Port Town Aero Dive FD");
            VFD.push_back("Onett FD");
            VFD.push_back("Castle Siege FD");
            VFD.push_back("Flat Zone X FD");
            VFD.push_back("Palutena's Temple FD");
            VFD.push_back("Skyworld FD");
            VFD.push_back("Gamer FD");
            VFD.push_back("Garden of Hope FD");
            VFD.push_back("Town and City FD");
            VFD.push_back("Smashville FD");
            VFD.push_back("Wii Fit Studio FD");
            VFD.push_back("Gaur Plain FD");
            VFD.push_back("Duck Hunt FD");
            VFD.push_back("Wrecking Crew FD");
            VFD.push_back("Pilotwings FD");
            VFD.push_back("Miiverse FD");
            VFD.push_back("Windy Hill Zone FD");
            VFD.push_back("Wily Castle FD");
            VFD.push_back("Pac-Land FD");
            VFD.push_back("Suzaku Castle FD");
            VFD.push_back("Midgar FD");
            VFD.push_back("Umbra Clock Tower FD");
        }
        else if (NbJoueur >= 8)
        {
            Vterrain.push_back("Battlefield");
            Vterrain.push_back("Big Battlefield");
            Vterrain.push_back("Final Destination");
            Vterrain.push_back("Mario Galaxy");
            Vterrain.push_back("Super Mario Maker");
            Vterrain.push_back("Peach's Castle (64)");
            Vterrain.push_back("Luigi's Mansion");
            Vterrain.push_back("Mario Circuit (Brawl)");
            Vterrain.push_back("Yoshi's Island");
            Vterrain.push_back("Kongo Jungle 64");
            Vterrain.push_back("Hyrule Castle (64)");
            Vterrain.push_back("Temple");
            Vterrain.push_back("Bridge of Eldin");
            Vterrain.push_back("Pyrosphere");
            Vterrain.push_back("Norfair");
            Vterrain.push_back("Dream Land (64)");
            Vterrain.push_back("Lylat Cruise");
            Vterrain.push_back("Pokemon Stadium 2");
            Vterrain.push_back("Onett");
            Vterrain.push_back("Castle Siege");
            Vterrain.push_back("Town and City");
            Vterrain.push_back("Smashville");
            Vterrain.push_back("Wii Fit Studio");
            Vterrain.push_back("Gaur Plain");
            Vterrain.push_back("Duck Hunt");
            Vterrain.push_back("Windy Hill Zone");
            Vterrain.push_back("Suzaku Castle");
            Vterrain.push_back("Midgar");
            Vterrain.push_back("Umbra Clock Tower");
            VFD.push_back("Battlefield FD");
            VFD.push_back("Big Battlefield FD");
            VFD.push_back("Mario Galaxy FD");
            VFD.push_back("Mushroom Kingdom U FD");
            VFD.push_back("Super Mario Maker FD");
            VFD.push_back("Peach's Castle (64) FD");
            VFD.push_back("Delfino Plaza FD");
            VFD.push_back("Luigi's Mansion FD");
            VFD.push_back("Mario Circuit (Brawl) FD");
            VFD.push_back("Yoshi's Island FD");
            VFD.push_back("Jungle Hijinxs FD");
            VFD.push_back("Kongo Jungle 64 FD");
            VFD.push_back("75m FD");
            VFD.push_back("Hyrule Castle (64) FD");
            VFD.push_back("Temple FD");
            VFD.push_back("Bridge of Eldin FD");
            VFD.push_back("Pirate Ship FD");
            VFD.push_back("Pyrosphere FD");
            VFD.push_back("Norfair FD");
            VFD.push_back("The Great Cave Offensive FD");
            VFD.push_back("Dream Land (64) FD");
            VFD.push_back("Halberd FD");
            VFD.push_back("Orbital Gate Assault FD");
            VFD.push_back("Lylat Cruise FD");
            VFD.push_back("Kalos Pokémon League FD");
            VFD.push_back("Pokémon Stadium 2 FD");
            VFD.push_back("Port Town Aero Dive FD");
            VFD.push_back("Onett FD");
            VFD.push_back("Castle Siege FD");
            VFD.push_back("Flat Zone X FD");
            VFD.push_back("Palutena's Temple FD");
            VFD.push_back("Skyworld FD");
            VFD.push_back("Gamer FD");
            VFD.push_back("Garden of Hope FD");
            VFD.push_back("Town and City FD");
            VFD.push_back("Smashville FD");
            VFD.push_back("Wii Fit Studio FD");
            VFD.push_back("Gaur Plain FD");
            VFD.push_back("Duck Hunt FD");
            VFD.push_back("Wrecking Crew FD");
            VFD.push_back("Pilotwings FD");
            VFD.push_back("Miiverse FD");
            VFD.push_back("Windy Hill Zone FD");
            VFD.push_back("Wily Castle FD");
            VFD.push_back("Pac-Land FD");
            VFD.push_back("Suzaku Castle FD");
            VFD.push_back("Midgar FD");
            VFD.push_back("Umbra Clock Tower FD");
        }
        else
        {

            Vterrain.push_back("Battlefield");
            Vterrain.push_back("Big Battlefield");
            Vterrain.push_back("Final Destination");
            Vterrain.push_back("Mario Galaxy");
            Vterrain.push_back("Mushroom Kingdom U");
            Vterrain.push_back("Mario Circuit");
            Vterrain.push_back("Super Mario Maker");
            Vterrain.push_back("Peach's Castle (64)");
            Vterrain.push_back("Delfino Plaza");
            Vterrain.push_back("Luigi's Mansion");
            Vterrain.push_back("Mario Circuit (Brawl)");
            Vterrain.push_back("Yoshi's Island");
            Vterrain.push_back("Kongo Jungle 64");
            Vterrain.push_back("Skyloft");
            Vterrain.push_back("Hyrule Castle (64)");
            Vterrain.push_back("Temple");
            Vterrain.push_back("Bridge of Eldin");
            Vterrain.push_back("Pirate Ship");
            Vterrain.push_back("Pyrosphere");
            Vterrain.push_back("Norfair");
            Vterrain.push_back("Dream Land (64)");
            Vterrain.push_back("Halberd");
            Vterrain.push_back("Lylat Cruise");
            Vterrain.push_back("Kalos Pokemon League");
            Vterrain.push_back("Pokemon Stadium 2");
            Vterrain.push_back("Port Town Aero Dive");
            Vterrain.push_back("Onett");
            Vterrain.push_back("Coliseum");
            Vterrain.push_back("Castle Siege");
            Vterrain.push_back("Flat Zone X");
            Vterrain.push_back("Skyworld");
            Vterrain.push_back("Gamer");
            Vterrain.push_back("Garden of Hope");
            Vterrain.push_back("Town and City");
            Vterrain.push_back("Smashville");
            Vterrain.push_back("Boxing Ring");
            Vterrain.push_back("Wii Fit Studio");
            Vterrain.push_back("Gaur Plain");
            Vterrain.push_back("Duck Hunt");
            Vterrain.push_back("Wrecking Crew");
            Vterrain.push_back("Pilotwings");
            Vterrain.push_back("Wuhu Island");
            Vterrain.push_back("Miiverse");
            Vterrain.push_back("Windy Hill Zone");
            Vterrain.push_back("Wily Castle");
            Vterrain.push_back("Pac-Land");
            Vterrain.push_back("Suzaku Castle");
            Vterrain.push_back("Midgar");
            Vterrain.push_back("Umbra Clock Tower");
            VFD.push_back("Battlefield FD");
            VFD.push_back("Big Battlefield FD");
            VFD.push_back("Final Destination");
            VFD.push_back("Mario Galaxy FD");
            VFD.push_back("Mushroom Kingdom U FD");
            VFD.push_back("Mario Circuit FD");
            VFD.push_back("Super Mario Maker FD");
            VFD.push_back("Peach's Castle (64) FD");
            VFD.push_back("Delfino Plaza FD");
            VFD.push_back("Luigi's Mansion FD");
            VFD.push_back("Mario Circuit (Brawl) FD");
            VFD.push_back("Yoshi's Island FD");
            VFD.push_back("Jungle Hijinxs FD");
            VFD.push_back("Kongo Jungle 64 FD");
            VFD.push_back("75m FD");
            VFD.push_back("Skyloft FD");
            VFD.push_back("Hyrule Castle (64) FD");
            VFD.push_back("Temple FD");
            VFD.push_back("Bridge of Eldin FD");
            VFD.push_back("Pirate Ship FD");
            VFD.push_back("Pyrosphere FD");
            VFD.push_back("Norfair FD");
            VFD.push_back("The Great Cave Offensive FD");
            VFD.push_back("Dream Land (64) FD");
            VFD.push_back("Halberd FD");
            VFD.push_back("Orbital Gate Assault FD");
            VFD.push_back("Lylat Cruise FD");
            VFD.push_back("Kalos Pokémon League FD");
            VFD.push_back("Pokémon Stadium 2 FD");
            VFD.push_back("Port Town Aero Dive FD");
            VFD.push_back("Onett FD");
            VFD.push_back("Coliseum FD");
            VFD.push_back("Castle Siege FD");
            VFD.push_back("Flat Zone X FD");
            VFD.push_back("Palutena's Temple FD");
            VFD.push_back("Skyworld FD");
            VFD.push_back("Gamer FD");
            VFD.push_back("Garden of Hope FD");
            VFD.push_back("Town and City FD");
            VFD.push_back("Smashville FD");
            VFD.push_back("Boxing Ring FD");
            VFD.push_back("Wii Fit Studio FD");
            VFD.push_back("Gaur Plain FD");
            VFD.push_back("Duck Hunt FD");
            VFD.push_back("Wrecking Crew FD");
            VFD.push_back("Pilotwings FD");
            VFD.push_back("Wuhu Island FD");
            VFD.push_back("Miiverse FD");
            VFD.push_back("Windy Hill Zone FD");
            VFD.push_back("Wily Castle FD");
            VFD.push_back("Pac-Land FD");
            VFD.push_back("Suzaku Castle FD");
            VFD.push_back("Midgar FD");
            VFD.push_back("Umbra Clock Tower FD");
        }
        ofstream Resultat ("Resultats/Matchs.txt", ios_base::app);
        switch (NbJoueur)
        {
            case 2 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1)
                        J2 = rand() % NbJoueur + 1;
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    pair <CCharacter, CCharacter> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second = J[i];
                            break;
                        }
                    }

                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;



                    cout << Match.first.GetJoueur() << " " << Match.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.GetJoueur() << " " << Match.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.NomJoueur() << " " << Match.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.NomJoueur() << " " << Match.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            case 3 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                        J[i].SetRound(J[i].GetRound()+1);
                    while (J2 == J1 || J2 == J3 || J3 == J1)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    pair <pair <CCharacter, CCharacter>, CCharacter> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;
                    cout << Match.first.first.GetJoueur() << " " << Match.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.GetJoueur() << " " << Match.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.GetJoueur() << " " << Match.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.NomJoueur() << " " << Match.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.NomJoueur() << " " << Match.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.NomJoueur() << " " << Match.second.GetChara() << endl << endl;

                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            case 4 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    unsigned J4;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    J4 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J4)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J1)
                            J4 = rand() % NbJoueur + 1;
                        while (J3 == J4)
                            J3 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    unsigned P4 = rand() %  MPerso[J4 - 1].size();
                    pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;

                    cout << Match.first.first.GetJoueur() << " " << Match.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.GetJoueur() << " " << Match.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.GetJoueur() << " " << Match.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.NomJoueur() << " " << Match.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.NomJoueur() << " " << Match.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.NomJoueur() << " " << Match.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J4)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            case 5 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    unsigned J4;
                    unsigned J5;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    J4 = rand() % NbJoueur + 1;
                    J5 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J3 == J5 || J4 == J5 || J1 == J5)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J4)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J1)
                            J4 = rand() % NbJoueur + 1;
                        while (J3 == J4)
                            J3 = rand() % NbJoueur + 1;
                        while (J2 == J5)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J5)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J5)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J1)
                            J5 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    unsigned P4 = rand() %  MPerso[J4 - 1].size();
                    unsigned P5 = rand() %  MPerso[J5 - 1].size();
                    pair <pair <pair <CCharacter, CCharacter>, CCharacter>,pair <CCharacter, CCharacter>> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;
                    cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.GetJoueur() << " " << Match.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.GetJoueur() << " " << Match.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.NomJoueur() << " " << Match.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.NomJoueur() << " " << Match.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J4)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J5)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            case 6 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    unsigned J4;
                    unsigned J5;
                    unsigned J6;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    J4 = rand() % NbJoueur + 1;
                    J5 = rand() % NbJoueur + 1;
                    J6 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5
                           || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J4)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J1)
                            J4 = rand() % NbJoueur + 1;
                        while (J3 == J4)
                            J3 = rand() % NbJoueur + 1;
                        while (J2 == J5)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J6)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J5)
                            J3 = rand() % NbJoueur + 1;
                        while (J3 == J6)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J5)
                            J4 = rand() % NbJoueur + 1;
                        while (J4 == J6)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J6)
                            J5 = rand() % NbJoueur + 1;
                        while (J1 == J6)
                            J6 = rand() % NbJoueur + 1;
                        while (J5 == J1)
                            J5 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    unsigned P4 = rand() %  MPerso[J4 - 1].size();
                    unsigned P5 = rand() %  MPerso[J5 - 1].size();
                    unsigned P6 = rand() %  MPerso[J6 - 1].size();
                    pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <CCharacter, CCharacter>> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;
                    cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.GetJoueur() << " " << Match.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.NomJoueur() << " " << Match.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J4)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J5)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J6)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            case 7 :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    unsigned J4;
                    unsigned J5;
                    unsigned J6;
                    unsigned J7;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    J4 = rand() % NbJoueur + 1;
                    J5 = rand() % NbJoueur + 1;
                    J6 = rand() % NbJoueur + 1;
                    J7 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5
                           || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6 || J2 == J7 || J3 == J7 || J4 == J7 || J5 == J7 || J6 == J7 || J1 == J7)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J4)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J1)
                            J4 = rand() % NbJoueur + 1;
                        while (J3 == J4)
                            J3 = rand() % NbJoueur + 1;
                        while (J2 == J5)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J6)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J5)
                            J3 = rand() % NbJoueur + 1;
                        while (J3 == J6)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J5)
                            J4 = rand() % NbJoueur + 1;
                        while (J4 == J6)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J6)
                            J5 = rand() % NbJoueur + 1;
                        while (J1 == J6)
                            J6 = rand() % NbJoueur + 1;
                        while (J5 == J1)
                            J5 = rand() % NbJoueur + 1;
                        while (J2 == J7)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J7)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J7)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J7)
                            J5 = rand() % NbJoueur + 1;
                        while (J6 == J7)
                            J6 = rand() % NbJoueur + 1;
                        while (J1 == J7)
                            J7 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    unsigned P4 = rand() %  MPerso[J4 - 1].size();
                    unsigned P5 = rand() %  MPerso[J5 - 1].size();
                    unsigned P6 = rand() %  MPerso[J6 - 1].size();
                    unsigned P7 = rand() %  MPerso[J7 - 1].size();
                    pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>, CCharacter>> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J7 && J[i].GetNumPerso() == P7))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;
                    cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.first.GetJoueur() << " " << Match.second.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.second.GetJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                         << "VS" << endl
                         << Match.second.second.GetJoueur() << " " << Match.second.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.first.NomJoueur() << " " << Match.second.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.second.NomJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                             << "VS" << endl
                             << Match.second.second.NomJoueur() << " " << Match.second.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J4)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J5)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J6)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J7)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }
            default :
            {
                for (unsigned Cmpt (0); Cmpt < NbMatch; Cmpt++)
                {
                    system("cls");
                    unsigned J1;
                    unsigned J2;
                    unsigned J3;
                    unsigned J4;
                    unsigned J5;
                    unsigned J6;
                    unsigned J7;
                    unsigned J8;
                    J1 = rand() % NbJoueur + 1;
                    J2 = rand() % NbJoueur + 1;
                    J3 = rand() % NbJoueur + 1;
                    J4 = rand() % NbJoueur + 1;
                    J5 = rand() % NbJoueur + 1;
                    J6 = rand() % NbJoueur + 1;
                    J7 = rand() % NbJoueur + 1;
                    J8 = rand() % NbJoueur + 1;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        J[i].SetRound(J[i].GetRound()+1);
                    }
                    while (J2 == J1 || J2 == J3 || J2 == J4 || J3 == J1 || J4 == J1 || J3 == J4 || J2 == J5 || J2 == J6 || J3 == J5 || J3 == J6 || J4 == J5 || J4 == J6 || J5 == J6 || J1 == J5 || J1 == J6 || J2 == J7 || J2 == J8 || J3 == J7 || J3 == J8 || J4 == J7 || J4 == J8 || J5 == J7 || J5 == J8 || J6 == J7 || J6 == J8 || J7 == J8 || J1 == J7 || J1 == J8)
                    {
                        while (J2 == J1)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J3)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J4)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J1)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J1)
                            J4 = rand() % NbJoueur + 1;
                        while (J3 == J4)
                            J3 = rand() % NbJoueur + 1;
                        while (J2 == J5)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J6)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J5)
                            J3 = rand() % NbJoueur + 1;
                        while (J3 == J6)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J5)
                            J4 = rand() % NbJoueur + 1;
                        while (J4 == J6)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J6)
                            J5 = rand() % NbJoueur + 1;
                        while (J1 == J6)
                            J6 = rand() % NbJoueur + 1;
                        while (J5 == J1)
                            J5 = rand() % NbJoueur + 1;
                        while (J2 == J7)
                            J2 = rand() % NbJoueur + 1;
                        while (J2 == J8)
                            J2 = rand() % NbJoueur + 1;
                        while (J3 == J7)
                            J3 = rand() % NbJoueur + 1;
                        while (J3 == J8)
                            J3 = rand() % NbJoueur + 1;
                        while (J4 == J7)
                            J4 = rand() % NbJoueur + 1;
                        while (J4 == J8)
                            J4 = rand() % NbJoueur + 1;
                        while (J5 == J7)
                            J5 = rand() % NbJoueur + 1;
                        while (J5 == J8)
                            J5 = rand() % NbJoueur + 1;
                        while (J6 == J7)
                            J6 = rand() % NbJoueur + 1;
                        while (J6 == J8)
                            J6 = rand() % NbJoueur + 1;
                        while (J1 == J7)
                            J7 = rand() % NbJoueur + 1;
                        while (J1 == J8)
                            J8 = rand() % NbJoueur + 1;
                        while (J7 == J8)
                            J8 = rand() % NbJoueur + 1;
                    }
                    unsigned P1 = rand() %  MPerso[J1 - 1].size();
                    unsigned P2 = rand() %  MPerso[J2 - 1].size();
                    unsigned P3 = rand() %  MPerso[J3 - 1].size();
                    unsigned P4 = rand() %  MPerso[J4 - 1].size();
                    unsigned P5 = rand() %  MPerso[J5 - 1].size();
                    unsigned P6 = rand() %  MPerso[J6 - 1].size();
                    unsigned P7 = rand() %  MPerso[J7 - 1].size();
                    unsigned P8 = rand() %  MPerso[J8 - 1].size();
                    pair <pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>,pair <pair <CCharacter, CCharacter>,pair <CCharacter, CCharacter>>> Match;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J1) && (J[i].GetNumPerso() == P1))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J2 && J[i].GetNumPerso() == P2))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J3) && (J[i].GetNumPerso() == P3))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J4 && J[i].GetNumPerso() == P4))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.first.second.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J5 && J[i].GetNumPerso() == P5))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J6 && J[i].GetNumPerso() == P6))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.first.second = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J7 && J[i].GetNumPerso() == P7))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second.first = J[i];
                            break;
                        }
                    }
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if ((J[i].GetPlayer() == J8 && J[i].GetNumPerso() == P8))
                        {
                            J[i].SetNbMatch(J[i].GetNbMatch() + 1);
                            Match.second.second.second = J[i];
                            break;
                        }
                    }
                    string Terrain;
                    unsigned RandTerrain;

                    RandTerrain = rand() % Vterrain.size();

                    Terrain = Vterrain[RandTerrain];

                    if (Terrain == "Final Destination") Terrain = VFD[rand() % VFD.size()];


                    cout << endl;
                    cout << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    cout << "Objet " << ((rand() % 3 == 1) ? "Active" : "Desactive") << endl;
                    cout << "Terrain : " << Terrain << endl << endl;
                    cout << Match.first.first.first.GetJoueur() << " " << Match.first.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.first.second.GetJoueur() << " " << Match.first.first.second.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.first.GetJoueur() << " " << Match.first.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.first.second.second.GetJoueur() << " " << Match.first.second.second.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.first.GetJoueur() << " " << Match.second.first.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.first.second.GetJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                         << "VS" << endl
                         << Match.second.second.first.GetJoueur() << " " << Match.second.second.first.GetChara() << endl
                         << "VS" << endl
                         << Match.second.second.second.GetJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;

                    Resultat << "Match #" << Match.first.first.first.GetRound() << " (FFA) :" << endl << endl;
                    Resultat << Match.first.first.first.NomJoueur() << " " << Match.first.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.first.second.NomJoueur() << " " << Match.first.first.second.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.first.NomJoueur() << " " << Match.first.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.first.second.second.NomJoueur() << " " << Match.first.second.second.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.first.NomJoueur() << " " << Match.second.first.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.first.second.NomJoueur() << " " << Match.second.first.second.GetChara() <<  endl
                             << "VS" << endl
                             << Match.second.second.first.NomJoueur() << " " << Match.second.second.first.GetChara() << endl
                             << "VS" << endl
                             << Match.second.second.second.NomJoueur() << " " << Match.second.second.second.GetChara() << endl << endl;
                    unsigned Gagnant;
                    cout << "Joueur gagnant : ";
                    cin >> Gagnant;
                    for (unsigned i (0); i < J.size(); ++i)
                    {
                        if (Gagnant == J1)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J2)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J3)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J4)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.first.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J5)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J6)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.first.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J7)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.first.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                        if (Gagnant == J8)
                            if ((J[i].GetPlayer() == Gagnant && J[i].GetNumPerso() == Match.second.second.second.GetNumPerso()))
                            {
                                J[i].SetNbVictoire(J[i].GetNbVictoire() + 1); //augmentation du nombre de victoire
                                Resultat << "Vainqueur : " << J[i].NomJoueur() << endl << endl
                                         << "*****************************************************************************************" << endl << endl;//augmentation du nombre de victoire
                            }
                    }
                }
                break;
            }//case
        }//switch
    }

    void RandomMatchType (vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, vector <vector <string>> MPerso) //Pas de listing pour ce mode
    {
        unsigned TypeMatch;
        system("cls");
        switch (NbJoueur) //les types de matchs dépendent du nombre de joueurs
        {
            case 2 :
            case 3 : //à 2 ou 3 joueurs, on ne peut faire que du 1v1 ou du FFA
            {
                for (unsigned i = 0; i < NbMatch; ++i)
                {
                    TypeMatch = rand() % 2 + 1; //aléatoire entre 1 et 2
                    switch (TypeMatch)
                    {
                        case 1 :
                            M1v1(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 2 :
                            FFA(J, NbJoueur, 1, MPerso);
                            break;
                    }
                    system("cls");
                }
                break;
            }
            case 4 :
            case 5 : //à 4 ou 5 joueurs, on ne peut faire que du 1v1, 2v2 ou FFA
            {
                for (unsigned i = 0; i < NbMatch; ++i)
                {
                    TypeMatch = rand() % 3 + 1; //aléatoire entre 1 et 3 ou 5
                    switch (TypeMatch)
                    {
                        case 1 :
                        case 5 :
                            M1v1(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 2 :
                            FFA(J, NbJoueur, 1, MPerso);
                            break;
                        case 3 :
                        case 4 :
                            M2v2(J, NbJoueur, 1, MPerso, false);
                            break;
                    }
                    system("cls");
                }
                break;
            }
            case 6 :
            case 7 : //à 6 ou 7 joueurs, on ne peut faire que du 1v1, 2v2, 3v3, 2v2v2 ou FFA
            {
                for (unsigned i = 0; i < NbMatch; ++i)
                {
                    TypeMatch = rand() % 5 + 1; //aléatoire entre 1 et 5
                    switch (TypeMatch)
                    {
                        case 1 :
                            M1v1(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 2 :
                            FFA(J, NbJoueur, 1, MPerso);
                            break;
                        case 3 :
                            M2v2(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 4 :
                            M3v3(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 5 :
                            M2v2x3(J, NbJoueur, 1, MPerso, false);
                            break;
                    }
                    system("cls");
                }
                break;
            }
            default : //dans tous les autres cas (le cas 1 joueur étant géré plus tôt) on peut faire tous les modes
            {
                for (unsigned i = 0; i < NbMatch; ++i)
                {
                    TypeMatch = rand() % 7 + 1; //aléatoire entre 1 et 7
                    switch (TypeMatch)
                    {
                        case 1 :
                            M1v1(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 2 :
                            FFA(J, NbJoueur, 1, MPerso);
                            break;
                        case 3 :
                            M2v2(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 4 :
                            M3v3(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 5 :
                            M2v2x3(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 6 :
                            M2v2x4(J, NbJoueur, 1, MPerso, false);
                            break;
                        case 7 :
                            M4v4(J, NbJoueur, 1, MPerso, false);
                            break;
                    }
                    system("cls");
                }
                break;
            }
        }
    }
}
