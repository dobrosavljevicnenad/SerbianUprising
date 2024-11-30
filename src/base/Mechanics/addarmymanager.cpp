#include "addarmymanager.h"

AddArmyManager::AddArmyManager()
    :  territoryCount(0), regionCount(0), cityCount(0) {}

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

int AddArmyManager::calculateTotalTroops(){
    totalTroops += territoryCount * territoryBonus;

    totalTroops *= (regionMultiplier * regionCount > 0 ? regionMultiplier * regionCount : 1);

    totalTroops += cityCount * cityBonus;

    return totalTroops;
}

void AddArmyManager::decreaseAvailableTroops(int troops) {
    totalTroops -= troops;
    if (totalTroops < 0) totalTroops = 0; // Prevent negative troops
}
