#include "player.h"

Player::Player(int id, ArmyType ArmyType)
    : playerId(id), army(ArmyType){}


int Player::getPlayerId() const{
    return playerId;
}


Player::Player() : playerId(0), army(ArmyType::HAJDUK){}

ArmyType Player::getArmyType() const{
    return army;
}

void Player::setPlayerId(int id ){
    playerId = id;
}

void Player::setArmyType(ArmyType armyType){
    army = armyType;
}



