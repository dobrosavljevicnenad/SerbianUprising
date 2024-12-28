#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "army.h"

#include <string>

class Player {

private:
    int playerId;
    ArmyType army;

public:
    Player();
    Player(int id, ArmyType armyType);
    int getPlayerId() const;
    ArmyType getArmyType() const;
    void setPlayerId(int id);
    void setArmyType(ArmyType armyType);


    static Player fromJson(int playerId, const std::string& armyType) {
        ArmyType type = (armyType == "HAJDUK") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        return Player(playerId, type);
    }


};

#endif
