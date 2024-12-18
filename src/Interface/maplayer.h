#pragma once

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "../base/Entities/Army.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QFont>
#include <QGraphicsSceneMouseEvent>


class MapLayer : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit MapLayer(QString labelName, const QString &imagePath, bool enableHover,  QGraphicsItem *parent = nullptr);

    void setTroopCount(int count);
    void setColor(const QColor &newColor);
    void setArmyColor(ArmyType armyType);
    QGraphicsTextItem *troopText;
    int getTroopCount() const;
    void setCurrentPlayer(int PlayerId);
    QPixmap get_m_originalPixmap();
    void setMainMode(bool mainMode);
    int getId();
signals:
    void layerClicked(MapLayer *layer);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    QColor getArmyColor();

private:
    int id;
    int troopCount;
    int currentPlayer;
    bool isMainMode = true;
    QPixmap m_originalPixmap;
    QColor ArmyColor;

public:
    QString labelName;

};

#endif // MAPLAYER_H
