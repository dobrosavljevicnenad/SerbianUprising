#ifndef SERVERGAMEMANAGER_H
#define SERVERGAMEMANAGER_H

#include "../base/Mechanics/turn.h"
#include "../base/graph/graph.hpp"
#include "../base/Entities/year.h"
#include "../base/Mechanics/eventhandle.h"

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
    QVector<QStringList> generateBattleResults();
    QJsonObject rootObj;
    void processLoadGame(QJsonObject& gameData);

private:
    std::unique_ptr<graph::Graph> graph;
    Turn turn;
    Year gameYear;
    EventHandle eventHandle;

signals:
    void init_serializedGraphReady(const QJsonObject &serializedGraph);
    void serializedGraphReady2(const QJsonObject &serializedGraph, const QJsonObject Results,const QJsonObject events);
    void loadGame_serializedGraphReady(QJsonObject& serializedGraph);

};

#endif // SERVERGAMEMANAGER_H
