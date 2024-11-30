#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../base/graph/graph.hpp"
#include"maplayer.h"
#include "../base/Mechanics/MoveArmy.h"
#include "../network/server.h"
#include "../network/client.h"

#include<QGraphicsScene>
#include<iostream>
#include<QObject>

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager(QGraphicsScene* scene);
    void initializeMap();

    void addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player);

    void CreateRegion();
    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);
    QMap<MapLayer*,graph::Vertex*> layerToVertex;

    void startServer();
    void connectClients();

    Server server;
    Client c_player1;
    Client c_player2;
signals:
    void layerClicked(MapLayer* layer);

private:
    QGraphicsScene* scene;
    graph::Graph g;
public:
    MoveArmy ma;
};

#endif // GAMEMANAGER_H
