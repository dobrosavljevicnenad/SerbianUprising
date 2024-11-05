#include "maplayer.h"

MapLayer::MapLayer(const QString &imagePath,bool enableHover = true, QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(imagePath), parent) {
    if(enableHover)
        setAcceptHoverEvents(true);
}

void MapLayer::setColor(const QColor &newColor) {
    QImage image = pixmap().toImage();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x<image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            if (color.alpha()>0){
                image.setPixelColor(x, y, newColor);
            }
        }
    }

    setPixmap(QPixmap::fromImage(image));
}


void MapLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event)  {
    setColor(QColor(255, 0, 0, 150));
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void MapLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setColor(QColor(235,180,90,255));
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}
