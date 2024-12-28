#ifndef TURN_H
#define TURN_H

#include "action.h"
#include "../graph/graph.hpp"
#include "movearmy.h"
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

    void addAction(int playerId, const Action& action);
    void executeTurn();
    void clearActionBuffers();
    QString GetCurrentAction(const Action& action);
    MoveArmy moveArmy;
    std::vector<Action>& getPlayerBuffer(int playerId);
    std::vector<Results> battlesResults;
    QJsonObject serializeResultsVector(const std::vector<Results>& resultsVector);

private:
    unsigned numBattles;
    std::vector<Action> player1Buffer;
    std::vector<Action> player2Buffer;
    graph::Graph& m_graph;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    int player1Recruits;
    int player2Recruits;
    void executePlayerAttacks(int playerId);
    void executePlayerMoves(const Action& action, bool& battleMusic);
    void playBattleMusic();

    void executeMoveAction(const Action& action);
    void executeAttackAction(const int playerId, const Action& action);
    void executePlaceAction(const Action& action);
    void executeEventAction(const int playerId, const Action& action);
private slots:
    void onBattleFinished(Results results);
    void onBattleCanceled();
};

#endif // TURN_H
