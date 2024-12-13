#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../base/graph/graph.hpp"
#include"maplayer.h"
#include "../base/Mechanics/MoveArmy.h"
#include "../base/Mechanics/turn.h"
#include "Items/customarrowitem.h"
#include "../base/Mechanics/addarmymanager.h"
#include "../network/server.h"
#include "../network/client.h"

#include<QGraphicsScene>
#include<iostream>
#include<QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <map>

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager(QGraphicsScene* scene);
 /*   void initializeMap();

    void updateLayersGraphics();
    void clearArrows();
    void clearExplosions();
    void drawArrow(int playerId, MapLayer* from, MapLayer* to, int number,int actionId);
    void addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player);

    void CreateRegion();
    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);
    void removeArrowByActionId(int actionId);
    void filterAndRedrawArrows(int currentPlayerId);
    void updateLayersId(int PlayerId);
    AddArmyManager& getArmyManager(int playerId);


private:
    std::vector<MapLayer*> explosions;
    std::map<int,AddArmyManager> armyManagers;
    std::vector<MapLayer*> layers;
    std::map<int,std::vector<CustomArrowItem*>> arrows;
    QGraphicsScene* scene;
    graph::Graph g;
public:
<<<<<<< HEAD
    Turn turn;
public slots:
    void printExplosion(Vertex *target);

*/
};

#endif // GAMEMANAGER_H
