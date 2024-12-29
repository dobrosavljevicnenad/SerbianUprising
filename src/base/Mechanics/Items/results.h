#ifndef RESULTS_H
#define RESULTS_H

#include "../../Entities/army.h"
#include "../../Entities/terrain.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

struct BattleRound {
    int roundNumber;
    int defenderHits;
    int attackerHits;
    int defenderRemaining;
    int attackerRemaining;
};
class Results {
public:
    Results();

    void recordRound(int defenderHit, int attackerHit, int defenderRemaining, int attackerRemaining);
    void setWinner(Army* winner);
    void setTargetVertexId(int id);
    void setDefenderNumber(int soldiers);
    void setAttackerNumber(int soldiers);
    void setArmyTypes(ArmyType defenderType, ArmyType attackerType);
    void printResults() const;

    const Army* getWinner() const;
    int getTargetVertexId() const;
    int getDefenderNumber() const;
    int getAttackerNumber() const;
    ArmyType getAttackerType() const;
    ArmyType getDefenderType() const;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

    TerrainType getTerrain() const;
    void setTerrain(TerrainType newTerrain);

    std::vector<BattleRound> getRounds() const;

    int defenderAdvantage;
    int defenderTerrainAdvantage;
    int defenderRiverCrossingAdvantage;
    int defenderMoraleAdvantage;
    int attackerAdvantage;
    int attackerTerrainAdvantage;
    int attackerMoraleAdvantage;
private:
    int defenderNumber;
    int attackerNumber;
    std::vector<BattleRound> rounds;
    Army* winner;
    ArmyType defenderType;
    ArmyType attackerType;
    int targetVertexId;
    TerrainType terrain;
};

#endif // RESULTS_H
