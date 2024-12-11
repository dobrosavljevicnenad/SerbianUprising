#include "Results.h"
#include <iostream>

Results::Results()
    : defenderNumber(0), attackerNumber(0), rounds(), winner(nullptr), targetVertexId(-1) {}

void Results::recordRound(int defenderHit, int attackerHit, int defenderRemaining, int attackerRemaining) {
    BattleRound round = {
        static_cast<int>(rounds.size()) + 1, // Round number
        defenderHit,
        attackerHit,
        defenderRemaining,
        attackerRemaining
    };
    rounds.push_back(round);
}

void Results::setWinner(Army* winner) {
    this->winner = winner;
}
void Results::setTargetVertexId(int id){
    this->targetVertexId = id;
}
void Results::setDefenderNumber(int soldiers){
    this->defenderNumber = soldiers;
}
void Results::setAttackerNumber(int soldiers){
    this->attackerNumber = soldiers;
}
void Results::setArmyTypes(ArmyType defenderType, ArmyType attackerType) {
    this->defenderType = defenderType;
    this->attackerType = attackerType;
}

void Results::printResults() const {
    std::cout << "Battle Results:\n";
    std::cout << "Defender Type: " << (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary") << "\n";
    std::cout << "Defender number of soldiers: " << defenderNumber << "\n";
    std::cout << "Attacker Type: " << (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary") << "\n";
    std::cout << "Attacker number of soldiers: " << attackerNumber << "\n";
    std::cout << "Winner: " << (winner ? (winner->armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary") : "None") << "\n";
    std::cout << "Target Vertex ID: " << targetVertexId << "\n";

    for (const auto& round : rounds) {
        std::cout << "Round " << round.roundNumber << ":\n";
        std::cout << "  Defender hits: " << round.defenderHits << "\n";
        std::cout << "  Attacker hits: " << round.attackerHits << "\n";
        std::cout << "  Defender remaining soldiers: " << round.defenderRemaining << "\n";
        std::cout << "  Attacker remaining soldiers: " << round.attackerRemaining << "\n";
    }
}

const Army* Results::getWinner() const {
    return winner;
}

int Results::getTargetVertexId() const {
    return targetVertexId;
}

int Results::getDefenderNumber() const{
    return defenderNumber;
}
int Results::getAttackerNumber() const{
    return attackerNumber;
}

ArmyType Results::getDefenderType() const
{
    return defenderType;
}

ArmyType Results::getAttackerType() const
{
    return attackerType;
}
