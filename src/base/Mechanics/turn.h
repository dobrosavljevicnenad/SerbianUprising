#ifndef TURN_H
#define TURN_H

#include "../graph/vertex.hpp"

#include <vector>
#include <string>


class Turn
{
public:
    Turn();

    void endTurn();
    void changePlayer();
    void addActionToBuffer(int playerId, const std::string& action);
    QString processBuffer();

    int getCurrentPlayer() const;

private:
    int currentPlayerId;
    std::vector<std::string>*player1Buffer;
    std::vector<std::string>*player2Buffer;

    void processFriendActions(std::vector<std::string>& buffer);
    void processEnemyActions(std::vector<std::string>& buffer);
};

#endif // TURN_H
