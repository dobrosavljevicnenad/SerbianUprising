#ifndef RESULTS_H
#define RESULTS_H

#include "../Entities/Army.h"
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
    void setArmyTypes(ArmyType defenderType, ArmyType attackerType);
    void printResults() const;

    const Army* getWinner() const;
    int getTargetVertexId() const;

private:
    std::vector<BattleRound> rounds;
    Army* winner;
    ArmyType defenderType;
    ArmyType attackerType;
    int targetVertexId;
};

#endif // RESULTS_H
