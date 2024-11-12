#pragma once

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QFont>



class MapLayer : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit MapLayer(const QString &imagePath, bool enableHover,  QGraphicsItem *parent = nullptr);

    void setTroopCount(int count);

    void setColor(const QColor &newColor);

    QGraphicsTextItem *troopText;

    int getTroopCount() const;



signals:
    void layerClicked(MapLayer *layer);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int troopCount;
    QPixmap m_originalPixmap;


};

#endif // MAPLAYER_H
