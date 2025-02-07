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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager(QGraphicsScene* scene);
    void initializeMap();

    void updateLayersGraphics();
    void clearArrows();
    void drawArrow(MapLayer* from, MapLayer* to, int number,int actionId);
    void addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player);

    void CreateRegion();
    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);
    QMap<MapLayer*,graph::Vertex*> layerToVertex;
    void removeArrowByActionId(int actionId);
    void updateLayersId(int PlayerId);

signals:
    void layerClicked(MapLayer* layer);

private:
    std::vector<MapLayer*> layers;
    std::vector<CustomArrowItem*> arrows;
    QGraphicsScene* scene;
    graph::Graph g;
public:
    Turn turn;
};

#endif // GAMEMANAGER_H
