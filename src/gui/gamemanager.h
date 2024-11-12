#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../graph/graph.hpp"
#include<QGraphicsScene>
#include<iostream>

class GameManager
{
public:
    GameManager(QGraphicsScene* scene);
    void initializeMap();
    void addLayer(MapLayer* layer, const std::string& label, Territory territory, Army army, Player player);

    void transferTroops(MapLayer* from, MapLayer* to, int troops);
    void printConnections(graph::Vertex* vertex);

private:
    QGraphicsScene* scene;
    graph::Graph g;
    QMap<MapLayer*,graph::Vertex*> layerToVertex;
};

#endif // GAMEMANAGER_H
