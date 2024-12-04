#ifndef CLIENTGAMEMANAGER_H
#define CLIENTGAMEMANAGER_H

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
#include "../base/Mechanics/addarmymanager.h"
#include "mapscene.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(QGraphicsScene* scene,  QObject* parent = nullptr);
    void initializeGraphics();
    void printConnections();
    void processDataFromServer(const QByteArray& data);
    //void updateGraphicsFromServerState(const QJsonObject& serverState);
    //void sendEndTurn();
    //void sendAction(const Action& action);
    //void updateGraphics();
    //QJsonObject serializeGameState() const;
    //void deserializeGameState(const QJsonObject &json);

    void setScene(MapScene *newScene);
    void setId(int id);

signals:
    void layerClicked(MapLayer* layer);

private:
    QGraphicsScene* scene;
    graph::Graph g;
    std::vector<MapLayer*> layers;
    AddArmyManager armyManagers;

public:
    int ClientId ;

};
#endif // CLIENTGAMEMANAGER_H
