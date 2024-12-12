#ifndef SERVERGAMEMANAGER_H
#define SERVERGAMEMANAGER_H

#include "../base/Mechanics/turn.h"
#include "../base/graph/graph.hpp"

#include<QGraphicsScene>
#include<iostream>
#include<QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <map>



class ServerGameManager : public QObject{

    Q_OBJECT
public:
    ServerGameManager(QObject* parent);
    void initializeGame();
    void startGame();
    void executeActions(const std::vector<Action> &actions1, int p1_id, const std::vector<Action> &actions2, int p2_id);
    //void processEndTurn();

private:
    std::unique_ptr<graph::Graph> graph;
    Turn turn;

signals:
    void serializedGraphReady(const QJsonObject &serializedGraph);
};

#endif // SERVERGAMEMANAGER_H
