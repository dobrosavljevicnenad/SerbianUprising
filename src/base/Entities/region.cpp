#include "region.h"

Region::Region() : city(nullptr), troopProductionRate(1) {}

Region::Region(const std::string& id, const std::string& name)
    : regionId(id), regionName(name), troopProductionRate(1) {}

const std::string& Region::getRegionId() const {
    return regionId;
}

const std::string& Region::getRegionName() const {
    return regionName;
}

bool Region::addLayer(MapLayer* layer, City* city) {
    territories.emplace_back(layer, city); // Add the MapLayer and its associated City
    return true;
}

int Region::getBattleFactor() const {
    return (city != nullptr) ? city->getBattleFactor() : 0;
}

int Region::generateTroops() const {
    return territories.size() * troopProductionRate;
}

const std::vector<std::pair<MapLayer*, City*>>& Region::getTerritories() const {
    return territories;
}
