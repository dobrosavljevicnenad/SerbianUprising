#include "Territory.h"

Territory::Territory(TerrainType terrain) : m_terrain(terrain) {
    setDefaultAdvantages(terrain);
}

// Function to set default advantages for each TerrainType
void Territory::setDefaultAdvantages(TerrainType terrain) {
    switch (terrain) {
    case TerrainType::FIELD:
        m_attackerAdvantage = 0;
        m_defenderAdvantage = 0;
        break;
    case TerrainType::MOUNTAIN:
        m_attackerAdvantage = -10;
        m_defenderAdvantage = 20;
        break;
    case TerrainType::HILL:
        m_attackerAdvantage = -5;
        m_defenderAdvantage = 10;
        break;
    case TerrainType::FOREST:
        m_attackerAdvantage = 7;
        m_defenderAdvantage = 15;
        break;
    default:
        m_attackerAdvantage = 0;
        m_defenderAdvantage = 0;
        break;
    }
}

TerrainType Territory::getTerrain() const {
    return m_terrain;
}

int Territory::getAttackerAdvantage() const {
    return m_attackerAdvantage;
}

int Territory::getDefenderAdvantage() const {
    return m_defenderAdvantage;
}

std::string Territory::to_string(TerrainType type) const{
    switch(type){
    case TerrainType::FIELD:
        return "field";
        break;
    case TerrainType::FOREST:
        return "forest";
        break;
    case TerrainType::HILL:
        return "hill";
        break;
    case TerrainType::MOUNTAIN:
        return "mountain";
        break;
    default:
        return "";
    }
}
