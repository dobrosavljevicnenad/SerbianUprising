#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Army.h"

#include <string>

class Player {

private:
    int playerId;
    ArmyType army;

public:
    // Constructor
    Player();
    Player(int id, ArmyType armyType);


    int getPlayerId() const;


    ArmyType getArmyType() const;

    void setPlayerId(int id);

    void setArmyType(ArmyType armyType);


};

#endif
