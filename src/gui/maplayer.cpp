#include "maplayer.h"

MapLayer::MapLayer(const QString &imagePath,bool enableHover = true, QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(imagePath), parent), troopText(new QGraphicsTextItem(this)), m_originalPixmap(QPixmap(imagePath)) {
    if(enableHover)
        setAcceptHoverEvents(true);

    troopText->setDefaultTextColor(Qt::black);
    troopText->setFont(QFont("Arial", 12, QFont::Bold));
    troopText->setPos(boundingRect().width() / 2 - troopText->boundingRect().width() / 2,
                      boundingRect().height() / 2 - troopText->boundingRect().height() / 2);
    troopText->setZValue(1);
}

void MapLayer::setTroopCount(int count) {
    troopText->setPlainText(QString::number(count));
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
    setPixmap(m_originalPixmap);
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}


