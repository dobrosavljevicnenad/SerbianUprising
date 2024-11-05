#ifndef BATTLE_H
#define BATTLE_H

#include "Army.h"
#include <cstdlib>
#include <ctime>

class Battle
{
public:
    Battle(Army& army1, Army& army2);
    void start();

    void setTerrainAdvantage(int advantage1, int advantage2);

private:
    Army& m_army1;
    Army& m_army2;

    int m_terrainAdvantage1 = 0;
    int m_terrainAdvantage2 = 0;

    int calculateHit(int strength, int terrainAdvantage, int soldiers);
};

#endif // BATTLE_H
