#ifndef MAPLAYER_H
#define MAPLAYER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
#include <QFont>


class MapLayer : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit MapLayer(const QString &imagePath, bool enableHover,  QGraphicsItem *parent = nullptr);

    void setTroopCount(int count);

    void setColor(const QColor &newColor);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QGraphicsTextItem *troopText;
};

#endif // MAPLAYER_H
