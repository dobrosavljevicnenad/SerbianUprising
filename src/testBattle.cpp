#include "Army.h"
#include "Battle.h"
#include "Strength.h"

// Compile with g++ -o battle testBattle.cpp Army.cpp Battle.cpp Strength.cpp -std=c++11
// Run: ./battle

int main() {
    Strength::instance().setBoost(ArmyType::PATRIOT, 25);
    Strength::instance().setBoost(ArmyType::JANISSARY, 0);

    Army patriotArmy(40, ArmyType::PATRIOT);
    Army janissaryArmy(70, ArmyType::JANISSARY);

    Battle battle(patriotArmy, janissaryArmy);
    battle.setTerrainAdvantage(10, -10);
    battle.start();

    return 0;
}
