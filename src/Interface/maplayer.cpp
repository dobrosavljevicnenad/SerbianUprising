#include "maplayer.h"

MapLayer::MapLayer(const QString &imagePath,bool enableHover = true, QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(imagePath), parent),
    troopText(new QGraphicsTextItem(this)),
    m_originalPixmap(QPixmap(imagePath)),
    currentPlayer(1){
    if(enableHover)
        setAcceptHoverEvents(true);

    troopText->setDefaultTextColor(Qt::black);
    troopText->setFont(QFont("Arial", 12, QFont::Bold));
    troopText->setPos(boundingRect().width() / 2 - troopText->boundingRect().width() / 2,
                      boundingRect().height() / 2 - troopText->boundingRect().height() / 2);
    troopText->setZValue(1);
}

void MapLayer::setTroopCount(int count) {
    troopCount = count;
    troopText->setPlainText(QString::number(count));
}

QPixmap MapLayer::get_m_originalPixmap(){
    QImage image = pixmap().toImage();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x<image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            if (color.red() < 60 && color.green() < 60 && color.blue() < 60 && color.alpha() > 0) {
                image.setPixelColor(x, y, QColor(0, 0, 0));
            } else if (color.alpha() > 0) {
                image.setPixelColor(x, y, QColorConstants::Gray);
            }
        }
    }

    return QPixmap::fromImage(image);
}


int MapLayer::getTroopCount() const {
    return troopCount;
}

void MapLayer::setColor(const QColor &newColor) {
    QImage image = pixmap().toImage();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x<image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            if (color.red() < 60 && color.green() < 60 && color.blue() < 60 && color.alpha() > 0) {
                image.setPixelColor(x, y, QColor(0, 0, 0));
            } else if (color.alpha() > 0) {
                image.setPixelColor(x, y, newColor);
            }
        }
    }

    setPixmap(QPixmap::fromImage(image));
}

void MapLayer::setArmyColor(ArmyType armyType) {
    switch (armyType) {
    case ArmyType::HAJDUK:
        ArmyColor = QColor(198,54,60,255);
        break;
    case ArmyType::JANISSARY:
        ArmyColor = QColor(0,149,48,255);
        break;
    default:
        ArmyColor = QColor(255, 255, 255);
        break;
    }
    setColor(ArmyColor);
}

QColor MapLayer::getArmyColor(){
    return ArmyColor;
}

void MapLayer::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if(currentPlayer == 1){
        if(getArmyColor() == QColor(198,54,60,255)){
            setColor(QColor(255, 0, 0, 150));
            QGraphicsPixmapItem::hoverEnterEvent(event);
        } else {
            return;
        }
    } else {
        if(getArmyColor() == QColor(0,149,48,255)){
            setColor(QColor(255, 0, 0, 150));
            QGraphicsPixmapItem::hoverEnterEvent(event);
        } else {
            return;
        }
    }
}

void MapLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (isMainMode) {
        setColor(ArmyColor);
    }
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void MapLayer::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit layerClicked(this);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}
void MapLayer::setMainMode(bool mainMode) {
    isMainMode = mainMode;
}
void MapLayer::setCurrentPlayer(int PlayerId){
    currentPlayer = PlayerId;
}
