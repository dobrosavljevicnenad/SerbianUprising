#include "testBattle.h"

void test_Battle() {
    Strength::instance().setBoost(ArmyType::PATRIOT, 25);
    Strength::instance().setBoost(ArmyType::JANISSARY, 0);

    Army patriotArmy(40, ArmyType::PATRIOT);
    Army janissaryArmy(70, ArmyType::JANISSARY);

    Battle battle(patriotArmy, janissaryArmy);
    battle.setTerrainAdvantage(10, -10);
    battle.start();
}
