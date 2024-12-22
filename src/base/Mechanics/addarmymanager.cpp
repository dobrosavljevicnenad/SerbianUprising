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

int AddArmyManager::newRecruits(Player player) {
    int recruit = 0;
    int recruits = 0;

    for (auto vertex : graph->vertices) {
        recruit = 0;
        bool isSameCulture = false;

        if (vertex.second->culture == CultureType::SERBIAN && player.getArmyType() == ArmyType::HAJDUK && vertex.second->army.armyType() == ArmyType::HAJDUK) {
            recruit += 5;
            isSameCulture = true;
        } else if (vertex.second->culture != CultureType::SERBIAN && player.getArmyType() == ArmyType::HAJDUK && vertex.second->army.armyType() == ArmyType::HAJDUK) {
            recruit += 1;
        } else if (vertex.second->culture == CultureType::TURKISH && player.getArmyType() == ArmyType::JANISSARY && vertex.second->army.armyType() == ArmyType::JANISSARY) {
            recruit += 3;
            isSameCulture = true;
        } else if (vertex.second->culture != CultureType::TURKISH && player.getArmyType() == ArmyType::JANISSARY && vertex.second->army.armyType() == ArmyType::JANISSARY) {
            recruit += 1;
        }


        Region* region = vertex.second->region;
        if (region != nullptr) {
            const std::string& regionId = region->getRegionId();
            auto it = regionOwnershipStatus.find(regionId);
            if (it != regionOwnershipStatus.end() && it->second && isSameCulture) {
                recruit*=2;
            }
        }
        recruits+=recruit;
    }

    return recruits;
}

void AddArmyManager::updateRegionOwnership(unsigned clientId, const QVector<Region*>& regions) {
    regionOwnershipStatus.clear();

    for (const auto& region : regions) {
        bool isOwned = true;

        for (const auto& [layer, city] : region->getTerritories()) {
            graph::Vertex* vertex = graph->get_vertex_by_label(layer->labelName);
            if (!vertex || vertex->player.getPlayerId() != static_cast<int>(clientId)) {
                isOwned = false;
                break;
            }
        }

        regionOwnershipStatus[region->getRegionId()] = isOwned;
    }
}

int AddArmyManager::calculateTotalTroops(){
    totalTroops += territoryCount * territoryBonus;

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
