#ifndef MAPLAYER_H
#define MAPLAYER_H

#include <QGraphicsPixmapItem>
#include <QColor>
#include <QObject>
#include <QGraphicsSceneHoverEvent>


class MapLayer : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit MapLayer(const QString &imagePath, bool enableHover,  QGraphicsItem *parent = nullptr);


    void setColor(const QColor &newColor);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // MAPLAYER_H
