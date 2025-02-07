#pragma once

#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "maplayer.h"

#include<QGraphicsScene>
#include<QList>

class MapScene : public QGraphicsScene {
public:
    explicit MapScene(QObject *parent = nullptr);

    void addLayer(MapLayer *layer);

    void highlightNeighbors(MapLayer *targetLayer, const QColor &highlightColor);

private:
    QList<MapLayer*> layers;

    bool isNeighbor(MapLayer *a, MapLayer *b);
};
#endif // MAPSCENE_H
