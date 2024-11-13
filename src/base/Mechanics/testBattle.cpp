#include "testBattle.h"

void test_Battle() {
    Strength::instance().setBoost(ArmyType::HAJDUK, 25);
    Strength::instance().setBoost(ArmyType::JANISSARY, 0);

    Army hajdukArmy(40, ArmyType::HAJDUK);
    Army janissaryArmy(70, ArmyType::JANISSARY);

    Territory mountainTerrain(TerrainType::MOUNTAIN);

    Battle battle(hajdukArmy, janissaryArmy);
    battle.setTerrainAdvantage(mountainTerrain.getDefenderAdvantage(),
                               mountainTerrain.getAttackerAdvantage());
    battle.start();
}
