#include "map.h"

Map::Map(QGraphicsScene* scene, const QMap<MapLayer*, graph::Vertex*>& qmapVertex)
    : scene(scene) {
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

void Map::generateCultureMap() {
    for (const auto& [layer, vertex] : layerToVertex) {
        QColor color = getColorForCulture(vertex->culture);
        layer->setColor(color);
    }
    scene->update();
}

QColor Map::getColorForCulture(CultureType culture) {
    switch (culture) {
    case CultureType::TURKISH: return QColor(255, 215, 0);
    case CultureType::SERBIAN: return QColor(139, 69, 19);
    case CultureType::GREEK: return QColor(0, 128, 255);
    case CultureType::ROMANIAN: return QColor(255, 0, 255);
    case CultureType::BULGARIAN: return QColor(0, 128, 0);
    default: return QColor(Qt::white);
    }
}

void Map::updateLayerToVertex(const QMap<MapLayer*, graph::Vertex*>& newLayerToVertex) {
    layerToVertex.clear();

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
        return QColor(Qt::lightGray);
    }

    switch (city->getLevel()) {
    case 1: return QColor(173, 216, 230);
    case 2: return QColor(100, 149, 237);
    case 3: return QColor(0, 0, 255);
    default: return QColor(Qt::gray);
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
        {"Srpski Pasaluk", QColor(255, 0, 0)},
        {"Bosanski Pasaluk", QColor(0, 255, 0)},
        {"Hercegovacki", QColor(0, 0, 255)},
        {"Janinski", QColor(255, 165, 0)},
        {"Solunski", QColor(128, 0, 128)},
        {"Manastirski", QColor(0, 255, 255)},
        {"Silistra", QColor(255, 192, 203)},
        {"Nish", QColor(139, 69, 19)},
        {"Edirne", QColor(255, 215, 0)},
        {"Wallachia", QColor(0, 128, 128)}
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
