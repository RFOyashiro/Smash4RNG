#pragma once
#include "CMatchs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "CCharacter.h"

namespace Tournament
{
    unsigned ClosestPow2 (unsigned Nombre);
    void Tournament1v1SE (std::vector<CCharacter> J, unsigned NbJoueur, std::vector <std::vector <std::string>> MPerso);
}

