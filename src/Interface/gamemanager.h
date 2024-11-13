#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../base/graph/graph.hpp"
#include"maplayer.h"

#include<QGraphicsScene>
#include<iostream>
#include<QObject>

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager(QGraphicsScene* scene);
    void initializeMap();
    void CreateRegion();
    void addLayer(MapLayer* layer, const std::string& label, Terrain territory, Army army, Player player);
    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);

signals:
    void layerClicked(MapLayer* layer);

private:
    QGraphicsScene* scene;
    graph::Graph g;
    QMap<MapLayer*,graph::Vertex*> layerToVertex;
};

#endif // GAMEMANAGER_H
