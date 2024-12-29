#pragma once

#ifndef BATTLE_H
#define BATTLE_H

#include "../Entities/army.h"
#include "Items/results.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

class Battle
{
public:
    Battle(Army& defender, Army& attacker);

    Results start();
    void setTerrainAdvantage(int defenderAdvantage, int riverAdvantage, int attackerAdvantage);

private:
    Army& m_defender;
    Army& m_attacker;

    int m_defenderAdvantage = 0;
    int m_attackerAdvantage = 0;
    int m_riverAdvantage = 0;
    int calculateHit(int strength, int terrainAdvantage, int soldiers);
    void printArmyStatus(const Army& army, const std::string& armyName) const;
    bool shouldRetreat(int attackerSoldiers, int defenderSoldiers, bool isAttacker) const;
};

#endif // BATTLE_H
