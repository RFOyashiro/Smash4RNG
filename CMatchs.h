#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CCharacter.h"

namespace Matchs
{
    void Stats (std::vector <CCharacter> J, unsigned NbJoueur, std::vector <std::vector <std::string>> MPerso);
    void M1v1 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void M2v2 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void M3v3 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void M4v4 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void M2v2x3 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void M2v2x4 (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso, bool Listing = true);
    void FFA (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso);
    void RandomMatchType (std::vector <CCharacter> & J, unsigned NbJoueur, unsigned NbMatch, std::vector <std::vector <std::string>> MPerso);
}
