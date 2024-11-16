#ifndef TURN_H
#define TURN_H

#include "Action.h"
#include "../graph/graph.hpp"
#include "MoveArmy.h"

#include <vector>
#include <string>
#include <sstream>
#include <QMediaPlayer>
#include <QAudioOutput>

class Turn {
public:
    explicit Turn(graph::Graph& graph);

    void addAction(int playerId, const Action& action);           // Add an action to a player's buffer
    void executeTurn();                                           // Execute all actions for both players
    void clearActionBuffers();                                        // Reset action buffers at the end of the turn
    void changePlayer();                                          // Switch the current player
    int getCurrentPlayerId() const;                              // Get the current player's ID
        QString bufferToString();  // New method to return the string from both buffers
    MoveArmy moveArmy;                    // Handles movement and battles

private:
    int currentPlayerId;                  // Tracks the current player (1 or 2)
    std::vector<Action> player1Buffer;    // Buffer for Player 1's actions
    std::vector<Action> player2Buffer;    // Buffer for Player 2's actions
    graph::Graph& m_graph;                // Reference to the game graph
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;

    void executePlayerActions(int playerId);                     // Execute all actions for a specific player
    std::vector<Action>& getPlayerBuffer(int playerId);          // Get the buffer for a specific player

    // Methods to handle specific action types
    void executeMoveAction(const Action& action);
    void executeAttackAction(const Action& action);
};

#endif // TURN_H
