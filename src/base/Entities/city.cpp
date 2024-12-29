#include "city.h"

City::City(int level, int battleFactor)
    : level(level), battleFactor(battleFactor) {}

bool City::isCityLayer() const {
    return level > 0;
}

int City::getBattleFactor() const {
    return battleFactor;
}

int City::getLevel() const {
    return level;
}

City* CityFactory::getCityByLevel(int level) {
    static  City cityLevel1(1, 5);
    static  City cityLevel2(2, 10);
    static  City cityLevel3(3, 20);

    switch (level) {
    case 1: return &cityLevel1;
    case 2: return &cityLevel2;
    case 3: return &cityLevel3;
    default: return nullptr;
    }
}
