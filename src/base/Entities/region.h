#pragma once

#ifndef REGION_H
#define REGION_H

#include "../../Interface/maplayer.h"
#include "city.h"
#include <vector>
#include <string>
#include <utility> // for std::pair

class Region {
public:
    Region();
    Region(const std::string& id, const std::string& name);

    const std::string& getRegionId() const;
    const std::string& getRegionName() const;
    bool addLayer(MapLayer* layer, City* city); // Updated to include City
    int getBattleFactor() const;
    int generateTroops() const;
    const std::vector<std::pair<MapLayer*, City*>>& getTerritories() const; // Return the updated vector

private:
    std::unique_ptr<City> city;
    int troopProductionRate;
    std::string regionId;
    std::string regionName;
    std::vector<std::pair<MapLayer*, City*>> territories; // Updated to store pairs of MapLayer and City
};

#endif // REGION_H
