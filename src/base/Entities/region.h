#ifndef REGION_H
#define REGION_H

#include <vector>
#include "../../Interface/maplayer.h"
#include "city.h"
class Region
{
public:
    Region();

    //~Region();

    bool addLayer(MapLayer*layer, std::unique_ptr<City> layerCity);
    int getBattleFactor() const;
    int generateTroops() const;

private:
    std::vector<MapLayer*>territories;
    std::unique_ptr<City> city;
    int troopProductionRate;
};

#endif // REGION_H
