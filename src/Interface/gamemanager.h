#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../base/graph/graph.hpp"
#include"maplayer.h"
#include "../base/Mechanics/MoveArmy.h"
#include "../base/Mechanics/turn.h"
#include "Items/customarrowitem.h"

#include<QGraphicsScene>
#include<iostream>
#include<QObject>

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager(QGraphicsScene* scene);
    void initializeMap();

    void updateLayersGraphics();
    void clearArrows();
    void drawArrow(MapLayer* from, MapLayer* to, int number);
    void addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player);

    void CreateRegion();
    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);
    QMap<MapLayer*,graph::Vertex*> layerToVertex;
signals:
    void layerClicked(MapLayer* layer);

private:
    std::vector<CustomArrowItem*> arrows;
    QGraphicsScene* scene;
    std::vector<MapLayer*> layers;
    graph::Graph g;
public:
    Turn turn;
};

#endif // GAMEMANAGER_H
