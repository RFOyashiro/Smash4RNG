#include "CTournament.h"

using namespace std;

namespace Tournament
{

    //ToDo : Classement Complet
    //ToDo : Double Elimination

    unsigned ClosestPow2t (unsigned Nombre)
    {
        unsigned Puissance = 0;
        unsigned PuissanceDe2 = 1;

        while (PuissanceDe2 < Nombre)
        {
            PuissanceDe2 *= 2;
            Puissance++;
        }
        return Puissance - 1;
    }



    void Tournament1v1SE(vector <CCharacter> J, unsigned NbJoueur, vector <vector <string>> MPerso)
    {
        unsigned MinPerso (90);
        for (unsigned i (0); i < MPerso.size(); ++i)
            if (MPerso[i].size() < MinPerso)
                MinPerso = MPerso[i].size();

        unsigned NbParticipant = MinPerso * NbJoueur;

        unsigned NbJoueurR1;
        unsigned NbJoueurR0;

        NbJoueurR1 = pow(2, ClosestPow2t(NbParticipant));
        NbJoueurR0 = NbParticipant - NbJoueurR1;

        unsigned NbJoueurR1Eff = NbJoueurR1 - NbJoueurR0;
        unsigned NbJoueurR0Eff = NbJoueurR0 + (NbJoueurR1 - NbJoueurR1Eff);

        vector <pair <CCharacter, CCharacter>> Matches;
        vector <pair <CCharacter, CCharacter>> MatchesR0;
        vector <CCharacter> Gagnant;
        vector <CCharacter> Participant;

        for (unsigned i (0); i < MPerso.size(); i++)
        {
            vector <string> Perso;

            while (Perso.size() != MinPerso)
            {
                bool In = false;
                unsigned RandChara;
                RandChara = rand() % MPerso[i].size();
                for (unsigned j (0); j < Perso.size(); ++j)
                    if (MPerso[i][RandChara] == Perso[j])
                    {
                        In = true;
                        break;
                    }
                if (!In)
                    Perso.push_back(MPerso[i][RandChara]);
            }

            for (unsigned j (0); j < Perso.size(); ++j)
                for (unsigned k (0); k < J.size(); ++k)
                {
                    if (J[k].GetChara() == Perso[j] && J[k].GetPlayer() == i + 1)
                        Participant.push_back(J[k]);
                }
        }

        unsigned Part (0);

        while (MatchesR0.size() < NbJoueurR0Eff / 2)
        {
            pair <CCharacter, CCharacter> Match;

            Match.first = Participant[Part];
            while (Match.first.GetRound() > 0)
            {
                if (Part == Participant.size() -1)
                    Part = 0;
                Match.first = Participant[++Part];
            }

            unsigned RandomNum;
            RandomNum = rand() % (Participant.size()); //nombre entre 0 et 3
            Match.second = Participant[RandomNum];

            while (Match.second.GetPlayer() == Match.first.GetPlayer() || Match.second.GetRound() > 0)
            {
                RandomNum = rand() % (Participant.size());
                Match.second = Participant[RandomNum];
            }

            Participant[Part].SetRound(Match.first.GetRound() + 1);
            Participant[RandomNum].SetRound(Match.second.GetRound() + 1);

            MatchesR0.push_back(Match);
        }

        //Déroulement des matchs Round 0 avec choix de match
        for (unsigned i = 0; i < MatchesR0.size(); ++i)
        {
            /*cout << endl;
                cout << "Match #" << i + 1 << " : ";
                cout << MatchesR0[i].first.NomJoueur() << " VS " << MatchesR0[i].second.NomJoueur() << endl;*/
            system("cls");
            cout << endl;
            cout << MatchesR0[i].first.GetJoueur() << " " <<MatchesR0[i].first.GetChara() << endl
                 << "VS " << endl
                 << MatchesR0[i].second.GetJoueur() << " " <<MatchesR0[i].second.GetChara() << endl << endl;

            unsigned JGagnant;
            cout << "Joueur gagnant : ";
            cin >> JGagnant;
            for (unsigned j (0); j < J.size(); ++j)
            {
                if (JGagnant == MatchesR0[i].first.GetPlayer())
                    if ((J[j].GetPlayer() == JGagnant && J[j].GetNumPerso() == MatchesR0[i].first.GetNumPerso()))
                    {
                        Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                        break;
                    }
                if (JGagnant == MatchesR0[i].second.GetPlayer())

                    if ((J[j].GetPlayer() == JGagnant && J[j].GetNumPerso() == MatchesR0[i].second.GetNumPerso()))
                    {
                        Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                        break;
                    }
            }
            system("cls");
        }

        for (unsigned i (0); i < Participant.size(); i++)
        {
            if (Participant[i].GetRound() == 0)
                Gagnant.push_back(Participant[i]);
        }

        unsigned Round = 0;

        while (Gagnant.size() > 2)
        {

            Round++;
            for (unsigned i (0); i < Gagnant.size(); i += 2)
            {
                pair <CCharacter, CCharacter> Match;
                Match.first = Gagnant[i];
                Match.second = Gagnant[i+1];
                Matches.push_back(Match);
                //cout << Gagnant[i].NomJoueur() << " " << Gagnant[i].GetChara() << endl;
                //cout << Gagnant[i+1].NomJoueur() << " " << Gagnant[i+1].GetChara() << endl;
            }

            vector <bool> Choix(Matches.size());
            for (unsigned i (0); i < Choix.size(); ++i)
                Choix[i] = false;

            Gagnant.clear();

            cout << "Round " << Round << " : " << endl << endl;

            for (unsigned i = 0; i < Matches.size(); ++i)
            {
                cout << endl;

                if (Matches[i].first.GetPlayer() == Matches[i].second.GetPlayer())
                {
                    Choix[i] = true;
                }
            }

            //Déroulement des matchs Round 1+ avec choix de match
            for (unsigned i = 0; i < Matches.size(); ++i)
            {

                if (Choix[i])
                {
                    cout << endl;
                    cout << Matches[i].first.NomJoueur() << " vous avez le choix entre : " << endl << endl
                         << Matches[i].first.GetChara() << " (" << Matches[i].first.GetNumPerso() << ")" << endl
                         << "ET" << endl
                         << Matches[i].second.GetChara() << " (" << Matches[i].second.GetNumPerso() << ")" << endl << endl;

                    cout << "Perso Choisi : ";
                    unsigned PGagnant;
                    cin >> PGagnant;

                    for (unsigned j (0); j < J.size(); ++j)
                    {
                        if (PGagnant == Matches[i].first.GetNumPerso())
                            if ((J[j].GetPlayer() == Matches[i].first.GetPlayer() && PGagnant == J[j].GetNumPerso()))
                            {
                                Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                                break;
                            }
                        if (PGagnant == Matches[i].second.GetNumPerso())

                            if ((J[j].GetPlayer() == Matches[i].second.GetPlayer() && PGagnant == J[j].GetNumPerso()))
                            {
                                Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                                break;
                            }
                    }
                }

                else
                {
                    cout << endl;
                    cout << Matches[i].first.GetJoueur() << " " <<Matches[i].first.GetChara() << endl
                         << "VS " << endl
                         << Matches[i].second.GetJoueur() << " " <<Matches[i].second.GetChara() << endl << endl;

                    unsigned JGagnant;
                    cout << "Joueur gagnant : ";
                    cin >> JGagnant;
                    for (unsigned j (0); j < J.size(); ++j)
                    {
                        if (JGagnant == Matches[i].first.GetPlayer())
                            if ((J[j].GetPlayer() == JGagnant && J[j].GetNumPerso() == Matches[i].first.GetNumPerso()))
                            {
                                Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                                break;
                            }
                        if (JGagnant == Matches[i].second.GetPlayer())

                            if ((J[j].GetPlayer() == JGagnant && J[j].GetNumPerso() == Matches[i].second.GetNumPerso()))
                            {
                                Gagnant.push_back(J[j]); //augmentation du nombre de victoire
                                break;
                            }
                    }
                }
                system("cls");
            }
            Matches.clear();
            Choix.clear();
        }

        if (Gagnant[0].GetJoueur() != Gagnant[1].GetJoueur())
        {

            cout << "Grand Final : " << endl << endl;
            cout << Gagnant[0].GetJoueur() << " " <<Gagnant[0].GetChara() << endl
                                           << "VS " << endl
                                           << Gagnant[1].GetJoueur() << " " <<Gagnant[1].GetChara() << endl << endl;

            unsigned JGagnant;
            cout << "Joueur gagnant : ";
            cin >> JGagnant;
            for (unsigned i (0); i < J.size(); ++i)
            {
                if (JGagnant == Gagnant[0].GetPlayer())
                    if ((J[i].GetPlayer() == JGagnant && J[i].GetNumPerso() == Gagnant[0].GetNumPerso()))
                    {
                        Gagnant.clear();
                        Gagnant.push_back(J[i]); //augmentation du nombre de victoire
                    }
                if (JGagnant == Gagnant[0].GetPlayer())

                    if ((J[i].GetPlayer() == JGagnant && J[i].GetNumPerso() == Gagnant[1].GetNumPerso()))
                    {
                        Gagnant.clear();
                        Gagnant.push_back(J[i]); //augmentation du nombre de victoire
                    }
            }
        }
        system("cls");
        cout << endl << endl << "Le gagnant est : " << Gagnant[0].NomJoueur() << " avec " << Gagnant[0].GetChara() << endl << endl;
        system("pause");
    }
}
