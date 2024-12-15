#pragma once

#ifndef REGION_H
#define REGION_H

#include "../../Interface/maplayer.h"
#include "city.h"

#include <vector>

class Region
{
public:
    Region();

    Region(const std::string& id,
           const std::string& name);

    //~Region();

    const std::string& getRegionId() const;
    const std::string& getRegionName() const;

    bool addLayer(MapLayer*layer, std::unique_ptr<City> layerCity);
    int getBattleFactor() const;
    int generateTroops() const;
    std::vector<MapLayer*>territories;

private:
    std::unique_ptr<City> city;
    int troopProductionRate;

    std::string regionId;
    std::string regionName;
};

#endif // REGION_H
