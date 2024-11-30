#include "addarmymanager.h"

AddArmyManager::AddArmyManager(int baseTroops)
    : baseTroops(baseTroops), territoryCount(0), regionCount(0), cityCount(0) {}

void AddArmyManager::addTerritory() {
    territoryCount++;
}

void AddArmyManager::removeTerritory() {
    if (territoryCount > 0) {
        territoryCount--;
    }
}

void AddArmyManager::addRegion() {
    regionCount++;
}

void AddArmyManager::removeRegion() {
    if (regionCount > 0) {
        regionCount--;
    }
}

void AddArmyManager::addCity() {
    cityCount++;
}

void AddArmyManager::removeCity() {
    if (cityCount > 0) {
        cityCount--;
    }
}

int AddArmyManager::calculateTotalTroops() const {
    int totalTroops = baseTroops;

    totalTroops += territoryCount * territoryBonus;

    totalTroops *= (regionMultiplier * regionCount > 0 ? regionMultiplier * regionCount : 1);

    totalTroops += cityCount * cityBonus;

    return totalTroops;
}
