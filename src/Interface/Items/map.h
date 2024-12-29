#pragma once

#include "../../base/graph/graph.hpp"
#include "../maplayer.h"
#include <QGraphicsScene>
#include <map>
#include <QMap>

class Map {
public:
    explicit Map(QGraphicsScene* scene, const QMap<MapLayer*, graph::Vertex*>& qmapVertex);

    ~Map() = default;

    void generateReliefMap();
    void generateRegionMap();
    void generateOwnerMap();
    void resetMainGameMap();
    void setMainMode(bool mainMode);

    void generateCityLevelMap();
    void updateLayerToVertex(const QMap<MapLayer*, graph::Vertex*>& newLayerToVertex);

    void generateCultureMap();
private:
    QGraphicsScene* scene;
    std::map<MapLayer*, graph::Vertex*> layerToVertex;

    QColor getColorForRelief(TerrainType terrain);
    QColor getColorForRegion(const std::string& regionName);
    QColor getColorForOwner(int playerId);
    QColor getColorForCulture(CultureType culture);
    QColor getColorForCityLevel(const City *city);
};
