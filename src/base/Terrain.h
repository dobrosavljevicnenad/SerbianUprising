#ifndef TERRITORY_H
#define TERRITORY_H

enum class TerrainType {
    FIELD,
    MOUNTAIN,
    HILL,
    FOREST
};

class Territory
{
public:
    explicit Territory(TerrainType terrain);

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

#endif // TERRITORY_H
