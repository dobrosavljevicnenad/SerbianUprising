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

QJsonObject Results::toJson() const {
    QJsonObject json;

    // Serialize simple fields
    json["defenderNumber"] = defenderNumber;
    json["attackerNumber"] = attackerNumber;
    json["targetVertexId"] = targetVertexId;
    json["defenderType"] = static_cast<int>(defenderType); // Assuming ArmyType is an enum
    json["attackerType"] = static_cast<int>(attackerType); // Assuming ArmyType is an enum
    json["terrain"] = static_cast<int>(terrain);          // Assuming TerrainType is an enum

    // Serialize winner
    json["winner"] = (winner && winner->armyType() == ArmyType::HAJDUK) ? "Hajduk" : "Janissary";

    // Serialize battle rounds
    QJsonArray roundsArray;
    for (const auto& round : rounds) {
        QJsonObject roundJson;
        roundJson["roundNumber"] = round.roundNumber;
        roundJson["defenderHits"] = round.defenderHits;
        roundJson["attackerHits"] = round.attackerHits;
        roundJson["defenderRemaining"] = round.defenderRemaining;
        roundJson["attackerRemaining"] = round.attackerRemaining;
        roundsArray.append(roundJson);
    }
    json["rounds"] = roundsArray;

    return json;
}

void Results::fromJson(const QJsonObject& json) {
    // Deserialize simple fields
    defenderNumber = json["defenderNumber"].toInt();
    attackerNumber = json["attackerNumber"].toInt();
    targetVertexId = json["targetVertexId"].toInt();
    defenderType = static_cast<ArmyType>(json["defenderType"].toInt());
    attackerType = static_cast<ArmyType>(json["attackerType"].toInt());
    terrain = static_cast<TerrainType>(json["terrain"].toInt());

    // Deserialize winner based on the string value
    QString winnerString = json["winner"].toString();
    if (winnerString == "Hajduk") {
        winner = new Army(ArmyType::HAJDUK); // Create a new Army object with the corresponding type
    } else if (winnerString == "Janissary") {
        winner = new Army(ArmyType::JANISSARY);
    } else {
        winner = nullptr;
    }

    // Deserialize battle rounds
    rounds.clear();
    QJsonArray roundsArray = json["rounds"].toArray();
    for (const auto& roundValue : roundsArray) {
        QJsonObject roundJson = roundValue.toObject();
        BattleRound round;
        round.roundNumber = roundJson["roundNumber"].toInt();
        round.defenderHits = roundJson["defenderHits"].toInt();
        round.attackerHits = roundJson["attackerHits"].toInt();
        round.defenderRemaining = roundJson["defenderRemaining"].toInt();
        round.attackerRemaining = roundJson["attackerRemaining"].toInt();
        rounds.push_back(round);
    }
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

TerrainType Results::getTerrain() const
{
    return terrain;
}

void Results::setTerrain(TerrainType newTerrain)
{
    terrain = newTerrain;
}

std::vector<BattleRound> Results::getRounds() const
{
    return rounds;
}

ArmyType Results::getAttackerType() const
{
    return attackerType;
}
