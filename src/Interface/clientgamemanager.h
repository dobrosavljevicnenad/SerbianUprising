#ifndef CLIENTGAMEMANAGER_H
#define CLIENTGAMEMANAGER_H

#include "../network/client.h"
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
#include "../base/Mechanics/turn.h"
#include "mapscene.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(Client* client, QGraphicsScene* scene,  QObject* parent = nullptr);
    void initializeGraphics();
    //void updateGraphicsFromServerState(const QJsonObject& serverState);
    //void sendEndTurn();
    //void sendAction(const Action& action);
    bool connectToServer();
    //void updateGraphics();
    //QJsonObject serializeGameState() const;
    //void deserializeGameState(const QJsonObject &json);

    void setScene(MapScene *newScene);

signals:
    void gameStarted();


private:
    QGraphicsScene* scene;
    graph::Graph g;  // Logical game state received from server
    Client* client;  // Client handles communication

};
#endif // CLIENTGAMEMANAGER_H
