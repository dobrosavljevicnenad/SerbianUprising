#ifndef TERRAIN_H
#define TERRAIN_H

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

private:
    TerrainType m_terrain;
    int m_attackerAdvantage;
    int m_defenderAdvantage;

    void setDefaultAdvantages(TerrainType terrain);
};

#endif // TERRAIN_H
