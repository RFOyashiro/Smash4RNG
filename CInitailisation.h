#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CCharacter.h"

namespace Initialisation
{
    std::vector <CCharacter> Init (unsigned NbJoueur, std::vector <std::vector <std::string>> MPerso, std::vector <std::string> NomJoueur);
    std::vector <std::vector <std::string>> InitPersoChoix (unsigned NbJoueur,std::vector <std::string> NomJoueur);
    std::vector <std::vector <std::string>> InitPerso (unsigned NbJoueur);
    std::vector <std::string> InitNomJoueur (unsigned NbJoueur);
    std::vector <std::string> InitNomJoueurBase(unsigned NbJoueur);
    void Launch ();
}
