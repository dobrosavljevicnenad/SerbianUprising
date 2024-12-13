#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>

enum class TerrainType {
    FIELD,
    MOUNTAIN,
    HILL,
    FOREST
};

class Terrain
{
public:
    explicit Terrain(TerrainType terrain);

    TerrainType getTerrain() const;
    int getAttackerAdvantage() const;
    int getDefenderAdvantage() const;

    void setAttackerAdvantage(int advantage);
    void setDefenderAdvantage(int advantage);

    std::string to_string(TerrainType type) const;

    static Terrain fromString(const std::string& type) {
        if (type == "MOUNTAIN") return Terrain(TerrainType::MOUNTAIN);
        if (type == "FOREST") return Terrain(TerrainType::FOREST);
        return Terrain(TerrainType::HILL);
    }

private:
    TerrainType m_terrain;
    int m_attackerAdvantage;
    int m_defenderAdvantage;

    void setDefaultAdvantages(TerrainType terrain);
};

#endif // TERRAIN_H
