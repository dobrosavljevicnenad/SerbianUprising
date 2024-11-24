#pragma once

#ifndef BATTLE_H
#define BATTLE_H

#include "../Entities/Army.h"


#include <ctime>
#include <iostream>
#include <cstdlib>

class Battle
{
public:
    Battle(Army& defender, Army& attacker);
    Army start();

    void setTerrainAdvantage(int defenderAdvantage, int attackerAdvantage);

private:
    Army& m_defender;
    Army& m_attacker;

    int m_defenderAdvantage = 0;
    int m_attackerAdvantage = 0;

    int calculateHit(int strength, int terrainAdvantage, int soldiers);
};

#endif // BATTLE_H
