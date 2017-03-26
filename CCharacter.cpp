#include "CCharacter.h"
#include <string>

std::string CCharacter::NomJoueur() const
{
    return m_NomJoueur;
}

void CCharacter::setNomJoueur(const std::string &NomJoueur)
{
    m_NomJoueur = NomJoueur;
}

CCharacter::CCharacter(const std::string & Chara, const unsigned & Player, const std::string & NomJoueur, const std::string & Bracket, const unsigned & Round, const unsigned & NumPerso, const unsigned & NbMatch, const unsigned & NbVictoire)
    : m_Chara (Chara), m_Player (Player), m_NomJoueur (NomJoueur), m_Bracket (Bracket), m_Round (Round), m_NumPerso (NumPerso), m_NbMatch (NbMatch), m_NbVictoire (NbVictoire) {}

std::string CCharacter::GetChara () {return m_Chara;}
unsigned CCharacter::GetPlayer () {return m_Player;}
std::string CCharacter::GetJoueur()
{
    std::string Name = m_NomJoueur;
    Name += " (";
    Name += std::to_string(m_Player);
    Name += ")";
    return Name;
}
std::string CCharacter::GetBracket () {return m_Bracket;}
unsigned CCharacter::GetRound () {return m_Round;}
unsigned CCharacter::GetNumPerso () {return m_NumPerso;}
unsigned CCharacter::GetNbMatch () {return m_NbMatch;}
unsigned CCharacter::GetNbVictoire() {return m_NbVictoire;}


void CCharacter::SetChara (std::string Chara) {m_Chara = Chara;}
void CCharacter::SetPlayer (unsigned Player) {m_Player = Player;}
void CCharacter::SetBracket (std::string Bracket) {m_Bracket = Bracket;}
void CCharacter::SetRound (unsigned Round) {m_Round = Round;}
void CCharacter::SetNumPerso (unsigned NumPerso) {m_NumPerso = NumPerso;}
void CCharacter::SetNbMatch (unsigned NbMatch) {m_NbMatch = NbMatch;}
void CCharacter::SetNbVictoire(unsigned NbVictoire) {m_NbVictoire = NbVictoire;}
