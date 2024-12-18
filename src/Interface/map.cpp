#include "map.h"

Map::Map(QGraphicsScene* scene, const QMap<MapLayer*, graph::Vertex*>& qmapVertex)
    : scene(scene) {
    // Convert QMap to std::map
    for (auto it = qmapVertex.begin(); it != qmapVertex.end(); ++it) {
        layerToVertex[it.key()] = it.value();
    }
}

void Map::generateReliefMap() {
    for (const auto& [layer, vertex] : layerToVertex) {
        QColor color = getColorForRelief(vertex->terrain.getTerrain());
        layer->setColor(color);
    }
    scene->update();
}

void Map::generateRegionMap() {
    for (const auto& [layer, vertex] : layerToVertex) {
        if (vertex->region) {
            QColor color = getColorForRegion(vertex->region->getRegionName());
            layer->setColor(color);
        }
    }
    scene->update();
}

void Map::updateLayerToVertex(const QMap<MapLayer*, graph::Vertex*>& newLayerToVertex) {
    // Clear the existing std::map
    layerToVertex.clear();

    // Convert QMap to std::map
    for (auto it = newLayerToVertex.begin(); it != newLayerToVertex.end(); ++it) {
        layerToVertex[it.key()] = it.value();
    }
}

void Map::generateCityLevelMap() {
    for (const auto& [layer, vertex] : layerToVertex) {
        QColor color = getColorForCityLevel(vertex->city);
        layer->setColor(color);
    }
    scene->update();
}

QColor Map::getColorForCityLevel(const City* city) {
    if (!city) {
        return QColor(Qt::lightGray); // Default color for non-city layers
    }

    switch (city->getLevel()) {
    case 1: return QColor(173, 216, 230); // Light Blue for Level 1
    case 2: return QColor(100, 149, 237); // Cornflower Blue for Level 2
    case 3: return QColor(0, 0, 255);     // Blue for Level 3
    default: return QColor(Qt::gray);     // Fallback color
    }
}

void Map::resetMainGameMap() {
    for (const auto& [layer, vertex] : layerToVertex) {
        ArmyType type = vertex->army.armyType();
        layer->setArmyColor(type);
    }
    scene->update();
}

QColor Map::getColorForRelief(TerrainType terrain) {
    switch (terrain) {
    case TerrainType::HILL: return QColor(150, 111, 51);
    case TerrainType::FIELD: return QColor(124, 252, 0);
    case TerrainType::FOREST: return QColor(34, 139, 34);
    case TerrainType::MOUNTAIN: return QColor(169, 169, 169);
    default: return QColor(Qt::white);
    }
}

QColor Map::getColorForRegion(const std::string& regionName) {
    static std::map<std::string, QColor> regionColors = {
        {"Srpski Pasaluk", QColor(255, 0, 0)},       // Red
        {"Bosanski Pasaluk", QColor(0, 255, 0)},     // Green
        {"Hercegovacki", QColor(0, 0, 255)},         // Blue
        {"Janinski", QColor(255, 165, 0)},           // Orange
        {"Solunski", QColor(128, 0, 128)},           // Purple
        {"Manastirski", QColor(0, 255, 255)},        // Cyan
        {"Silistra", QColor(255, 192, 203)},         // Pink
        {"Nish", QColor(139, 69, 19)},               // SaddleBrown
        {"Edirne", QColor(255, 215, 0)},             // Gold
        {"Wallachia", QColor(0, 128, 128)}           // Teal
    };

    auto it = regionColors.find(regionName);
    return (it != regionColors.end()) ? it->second : QColor(Qt::gray);
}

void Map::setMainMode(bool mainMode) {
    for (auto& [layer, vertex] : layerToVertex) {
        layer->setMainMode(mainMode);
    }
}

QColor Map::getColorForOwner(int playerId) {
    switch (playerId) {
    case 1: return QColor(Qt::blue);
    case 2: return QColor(Qt::red);
    default: return QColor(Qt::gray);
    }
}
