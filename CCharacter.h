#pragma once
#include <string>

class CCharacter
{
  private :
    std::string m_Chara;
    unsigned m_Player;
    std::string m_NomJoueur;
    std::string m_Bracket;
    unsigned m_Round;
    unsigned m_NumPerso;
    unsigned m_NbMatch;
    unsigned m_NbVictoire;

  public :
    CCharacter (const std::string & Chara = "a", const unsigned & Player = 0, const std::string & NomJoueur = "", const std::string & Bracket = "Winner",
                const unsigned & Round = 0, const unsigned & NumPerso = 0, const unsigned & NbMatch = 0, const unsigned & NbVictoire = 0);
    std::string GetChara ();
    unsigned GetPlayer ();
    std::string GetJoueur();
    std::string GetBracket ();
    unsigned GetRound ();
    unsigned GetNumPerso ();
    unsigned GetNbMatch();
    unsigned GetNbVictoire();
    void SetChara (std::string Chara);
    void SetPlayer (unsigned Player);
    void SetBracket (std::string Bracket);
    void SetRound (unsigned Round);
    void SetNumPerso (unsigned NumPerso);
    void SetNbMatch(unsigned NbMatch);
    void SetNbVictoire(unsigned NbVictoire);
    std::string NomJoueur() const;
    void setNomJoueur(const std::string &NomJoueur);
};
