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
#include "../base/Entities/player.h"


class ClientGameManager : public QObject{

    Q_OBJECT

public:
    explicit ClientGameManager(QGraphicsScene* scene,  QObject* parent = nullptr);
    void initializeGraphics();
    void printConnections();
    void processDataFromServer(const QByteArray& data);
    void updateGraphics();
    void EndTurnClicked(const QVector<Action>& actions, int id);
    void armyManagerReset();
    void drawArrow(int playerId, MapLayer* from, MapLayer* to, int number,int actionId);
    void addAction(const Action& action);
    QString GetCurrentAction(const Action& action);
    AddArmyManager& getArmyManager();
    void removeActionById(int actionId);
    void removeArrowByActionId(int actionId);
    void clearArrows();
    void setScene(MapScene *newScene);
    void setId(int id);


    QMap<MapLayer*,graph::Vertex*> layerToVertex;

signals:
    void layerClicked(MapLayer* layer);
    void endTurnActionsReady(const QVector<Action>& actions, int id);

private:
    bool init = false;
    QGraphicsScene* scene;
    std::unique_ptr<graph::Graph> clientGraph;
    std::vector<MapLayer*> layers;
    AddArmyManager armyManager;
    std::map<int,std::vector<CustomArrowItem*>> arrows;
    Player player;

public:
    int ClientId;
    QVector<Action> actionBuffer;

};
#endif // CLIENTGAMEMANAGER_H
