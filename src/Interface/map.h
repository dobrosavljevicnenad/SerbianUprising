#pragma once

#include "../base/graph/graph.hpp"      // Access to vertices
#include "maplayer.h"                   // Access to MapLayer
#include <QGraphicsScene>
#include <map>
#include <QMap>                         // For QMap compatibility

class Map {
public:
    // New constructor that accepts QMap
    explicit Map(QGraphicsScene* scene, const QMap<MapLayer*, graph::Vertex*>& qmapVertex);

    ~Map() = default;

    void generateReliefMap();
    void generateRegionMap();
    void generateOwnerMap();
    void resetMainGameMap(); // Reset to the main map with original colors
    void setMainMode(bool mainMode);

    void generateCityLevelMap();
    void updateLayerToVertex(const QMap<MapLayer*, graph::Vertex*>& newLayerToVertex);

private:
    QGraphicsScene* scene;
    std::map<MapLayer*, graph::Vertex*> layerToVertex;

    QColor getColorForRelief(TerrainType terrain);
    QColor getColorForRegion(const std::string& regionName);
    QColor getColorForOwner(int playerId);
    QColor getColorForCityLevel(const City *city);
};
