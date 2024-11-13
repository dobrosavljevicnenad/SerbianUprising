#include "region.h"

Region::Region() : city(nullptr), troopProductionRate(1) {}

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

int Region::getBattleFactor() const {
    return (city != nullptr) ? city->getBattleFactor() : 0;
}

int Region::generateTroops() const {
    return territories.size() * troopProductionRate;
}
