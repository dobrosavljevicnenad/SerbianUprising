#pragma once

#ifndef MERGEARMIES_H
#define MERGEARMIES_H

#include "player.h"
#include "Army.h"
#include "../Mechanics/Battle.h"

#include<iostream>
#include <vector>

class MergeArmies {
public:
    MergeArmies(std::vector<Army>&armies);
    void setTerrainAdvantage(int defenderAdvantage, int attackerAdvantage);
    Army battleArmies(Player &player1,Player &player2, std::vector<Army>&armies);

  private:
    std::vector<Army>armies;
    int m_defenderAdvantage = 0;
    int m_attackerAdvantage = 0;
};

#endif // MERGEARMIES_H
