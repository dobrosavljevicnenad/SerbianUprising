#include "mapscene.h"

MapScene::MapScene(QObject *parent) : QGraphicsScene(parent) {}

void MapScene::addLayer(MapLayer *layer) {
    addItem(layer);
    layers.append(layer);
}

void MapScene::highlightNeighbors(MapLayer *targetLayer, const QColor &highlightColor) {
    for (MapLayer *layer : layers) {
        if (isNeighbor(targetLayer, layer)) {
            layer->setColor(highlightColor);
        }
    }
}

bool MapScene::isNeighbor(MapLayer *a, MapLayer *b) {
    return true;
}
