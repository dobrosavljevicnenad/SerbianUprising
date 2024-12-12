#ifndef TURN_H
#define TURN_H

#include "Action.h"
#include "../graph/graph.hpp"
#include "MoveArmy.h"
#include <QObject>
#include <vector>
#include <string>
#include <sstream>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QEventLoop>
#include <iostream>
#include <ctime>

class Turn : public QObject {
    Q_OBJECT
public:
    explicit Turn(graph::Graph& graph);

    void addAction(int playerId, const Action& action);           // Add an action to a player's buffer
    void executeTurn();                                           // Execute all actions for both players
    void clearActionBuffers();                                    // Reset action buffers at the end of the turn
    void changePlayer();                                          // Switch the current player
    int getCurrentPlayerId() const;                               // Get the current player's ID
    QString GetCurrentAction(const Action& action);               // Get the description of the current action
    void removeActionById(int actionId);                          // Remove action by its ID
    MoveArmy moveArmy;                                            // Handles movement and battles
    std::vector<Action>& getPlayerBuffer(int playerId);           // Get the buffer for a specific player
    int getTurn();                                                // Get the current turn number
    std::vector<Results> battlesResults;
signals:
    void printExplosion(Vertex *target);

private:
    unsigned turn;                                                // Current turn number
    unsigned numBattles;
    int currentPlayerId;                                          // Tracks the current player (1 or 2)
    std::vector<Action> player1Buffer;                            // Buffer for Player 1's actions
    std::vector<Action> player2Buffer;                            // Buffer for Player 2's actions
    graph::Graph& m_graph;                                        // Reference to the game graph
    QMediaPlayer m_mediaPlayer;                                   // Media player for playing sounds
    QAudioOutput m_audioOutput;                                   // Audio output for sound control
    void updateSoldiersForPlayer(int playerId);
    void executePlayerAttacks(int playerId);                      // Execute all actions for a specific player
    void executePlayerMoves(const Action& action, bool& battleMusic);  // Execute an action and handle battle music if necessary
    void playBattleMusic();                                       // Play battle music if an attack happens
    void executeMoveAction(const Action& action);                 // Handle move actions
    void executeAttackAction(const int playerId, const Action& action); // Handle attack actions
private slots:
    void onBattleFinished(Results results);

};

#endif // TURN_H
