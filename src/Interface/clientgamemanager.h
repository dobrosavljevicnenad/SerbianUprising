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
#include "Items/customarrowitem.h"
#include "mapscene.h"
#include "../base/Mechanics/Action.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(QGraphicsScene* scene,  QObject* parent = nullptr);
    void initializeGraphics();
    void printConnections();
    void processDataFromServer(const QByteArray& data);
    void updateGraphics();
    //void updateGraphicsFromServerState(const QJsonObject& serverState);
    //void sendEndTurn();
    //void sendAction(const Action& action);
    //QJsonObject serializeGameState() const;
    //void deserializeGameState(const QJsonObject &json);
    void drawArrow(int playerId, MapLayer* from, MapLayer* to, int number,int actionId);
    void addAction(const Action& action);
    QString GetCurrentAction(const Action& action);
    AddArmyManager& getArmyManager(int playerId);


    void setScene(MapScene *newScene);
    void setId(int id);

    QMap<MapLayer*,graph::Vertex*> layerToVertex;

signals:
    void layerClicked(MapLayer* layer);

private:
    bool init = false;
    QGraphicsScene* scene;
    std::unique_ptr<graph::Graph> clientGraph;
    std::vector<MapLayer*> layers;
    //std::map<int,std::vector<CustomArrowItem*>> arrows;
    std::map<int,AddArmyManager> armyManagers;
    std::map<int,std::vector<CustomArrowItem*>> arrows;

public:
    int ClientId;
    std::vector<Action> actionBuffer;

};
#endif // CLIENTGAMEMANAGER_H
