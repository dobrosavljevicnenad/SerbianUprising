#pragma once

#ifndef REGION_H
#define REGION_H

#include "../../Interface/maplayer.h"
#include "city.h"
#include <vector>
#include <string>
#include <utility>

class Region {
public:
    Region();
    Region(const std::string& id, const std::string& name);

    const std::string& getRegionId() const;
    const std::string& getRegionName() const;
    bool addLayer(MapLayer* layer, City* city);
    int getBattleFactor() const;
    int generateTroops() const;
    const std::vector<std::pair<MapLayer*, City*>>& getTerritories() const;
    void setCityForLayer(MapLayer* layer, City* city);

private:
    std::unique_ptr<City> city;
    int troopProductionRate;
    std::string regionId;
    std::string regionName;
    std::vector<std::pair<MapLayer*, City*>> territories;
};

#endif // REGION_H
