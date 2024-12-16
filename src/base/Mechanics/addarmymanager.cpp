#include "addarmymanager.h"

AddArmyManager::AddArmyManager(graph::Graph* graph)
    : graph(graph), territoryCount(0), regionCount(0), cityCount(0) {}

void AddArmyManager::addTerritory(Player player) {
    territoryCount = newRecruits(player);
}

void AddArmyManager::removeTerritory() {
    if (territoryCount > 0) {
        territoryCount--;
    }
}

int AddArmyManager::newRecruits(Player player)
{
    int recruits = 0;
    for (auto vertex : graph->vertices) {
        if(vertex.second->culture == CultureType::SERBIAN && player.getArmyType() == ArmyType::HAJDUK && ArmyType::HAJDUK == vertex.second->army.armyType())
            recruits += 5;
        if(vertex.second->culture != CultureType::SERBIAN && player.getArmyType() == ArmyType::HAJDUK && ArmyType::HAJDUK == vertex.second->army.armyType())
            recruits += 1;
        if(vertex.second->culture == CultureType::TURKISH && player.getArmyType() == ArmyType::JANISSARY && ArmyType::JANISSARY == vertex.second->army.armyType())
            recruits += 3;
        if(vertex.second->culture != CultureType::TURKISH && player.getArmyType() == ArmyType::JANISSARY && ArmyType::JANISSARY == vertex.second->army.armyType())
            recruits += 1;
    }
    return recruits;
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
    if (totalTroops < 0) totalTroops = 0;
}

void AddArmyManager::endTurn(){
    totalTroops = 5;
}
