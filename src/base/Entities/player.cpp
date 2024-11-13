#include "player.h"

// Constructor
Player::Player(int id, ArmyType armyType)
    : playerId(id), army(armyType) {}

int Player::getPlayerId() const {
    return playerId;
}

Player::Player(){}

ArmyType Player::getArmyType() const {
    return army;
}

void Player::setPlayerId(int id) {
    playerId = id;
}

void Player::setArmyType(ArmyType armyType) {
    army = armyType;
}
