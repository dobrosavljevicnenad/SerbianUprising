#ifndef SERVERGAMEMANAGER_H
#define SERVERGAMEMANAGER_H

#include "../network/server.h"
#include "../base/Mechanics/turn.h"
#include "../base/Mechanics/addarmymanager.h"
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
    ServerGameManager();
    void initializeMap();
    //void processEndTurn();
    //void sendSerializedGameStateToClients();
    //void applyActions(std::vector<Action> actions);
    void startServer();
    //void processServerData(const QString &data);
    //void onGameStart();
    //QJsonObject serializeGameState() const;
    //void deserializeGameState(const QJsonObject &json);


signals:
    void gameStarted();

private:
    Graph g;
    Turn turn;
    std::map<int, AddArmyManager> armyManagers;
    Server server;
    ;  // Server handles communication
};

#endif // SERVERGAMEMANAGER_H
