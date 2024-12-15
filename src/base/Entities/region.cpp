#include "region.h"

Region::Region() : city(nullptr), troopProductionRate(1) {}


Region::Region(const std::string& id,
               const std::string& name)
    : regionId(id),
    regionName(name)
{}


bool Region::addLayer(MapLayer* layer, std::unique_ptr<City> layerCity) {
    if (territories.size() < 5) {
        territories.push_back(layer);
        if (layerCity->isCityLayer() && !city) {
            city = std::move(layerCity); // Only one city per region
        }
        return true;
    }
    return false;
}

const std::string& Region::getRegionId() const {
    return regionId;
}

const std::string& Region::getRegionName() const {
    return regionName;
}


int Region::getBattleFactor() const {
    return (city != nullptr) ? city->getBattleFactor() : 0;
}

int Region::generateTroops() const {
    return territories.size() * troopProductionRate;
}
