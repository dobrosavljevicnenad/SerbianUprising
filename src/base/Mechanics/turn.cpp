#include "turn.h"

Turn::Turn() : currentPlayerId(1) {}

void Turn::endTurn() {
    std::string result = processBuffer().toStdString();

    std::cout << "Processed actions for Player " << currentPlayerId << "    :\n" << result << std::endl;

    player1Buffer->clear();
    player2Buffer->clear();

    currentPlayerId = (currentPlayerId == 1) ? 2 : 1;
}

void Turn::changePlayer() {
    currentPlayerId = (currentPlayerId == 1) ? 2 : 1;
}

void Turn::addActionToBuffer(int playerId, const std::string& action) {
    if (playerId == 1) {
        std::cout << "move or attack" << std::endl;
        player1Buffer->push_back(action);
    } else {  
        player2Buffer->push_back(action);
        std::cout << player2Buffer->front() << std::endl;
    }
}

int Turn::getCurrentPlayer() const {
    return currentPlayerId;
}

QString Turn::processBuffer() {
    QString output;
    output += "Processing actions for Player 1:\n";
    processFriendActions(*player1Buffer);
    processEnemyActions(*player1Buffer);
    output += "Processing actions for Player 2:\n";
    processFriendActions(*player2Buffer);
    processEnemyActions(*player2Buffer);
    return output;
}

void Turn::processFriendActions(std::vector<std::string>& buffer) {
    for (const auto& action : buffer) {
        //... needs implementing
    }
}

void Turn::processEnemyActions(std::vector<std::string>& buffer) {
    for (const auto& action : buffer) {
        //... needs implementing
    }
}
