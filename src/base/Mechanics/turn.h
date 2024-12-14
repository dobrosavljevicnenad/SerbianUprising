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
    QString GetCurrentAction(const Action& action);               // Get the description of the current action
    MoveArmy moveArmy;                                            // Handles movement and battles
    std::vector<Action>& getPlayerBuffer(int playerId);           // Get the buffer for a specific player
    std::vector<Results> battlesResults;
    QJsonObject serializeResultsVector(const std::vector<Results>& resultsVector);

private:
    unsigned numBattles;
    std::vector<Action> player1Buffer;                            // Buffer for Player 1's actions
    std::vector<Action> player2Buffer;                            // Buffer for Player 2's actions
    graph::Graph& m_graph;                                        // Reference to the game graph
    QMediaPlayer m_mediaPlayer;                                   // Media player for playing sounds
    QAudioOutput m_audioOutput;                                   // Audio output for sound control

    void executePlayerAttacks(int playerId);                      // Execute all actions for a specific player
    void executePlayerMoves(const Action& action, bool& battleMusic);  // Execute an action and handle battle music if necessary

    void playBattleMusic();                                       // Play battle music if an attack happens

    void executeMoveAction(const Action& action);                 // Handle move actions
    void executeAttackAction(const int playerId, const Action& action); // Handle attack actions
    void executePlaceAction(const Action& action);
private slots:
    void onBattleFinished(Results results);
    void onBattleCanceled();
};

#endif // TURN_H
