#include "maplayer.h"

MapLayer::MapLayer(QString labelName, const QString &imagePath,bool enableHover = true, QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(imagePath), parent),
    troopText(new QGraphicsTextItem(this)),
    m_originalPixmap(QPixmap(imagePath)),
    currentPlayer(1),
    labelName(labelName){
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

    if (image.format() != QImage::Format_RGBA8888) {
        image = image.convertToFormat(QImage::Format_RGBA8888);
    }

    uchar* imageData = image.bits();
    int bytesPerLine = image.bytesPerLine();

    int newR = newColor.red();
    int newG = newColor.green();
    int newB = newColor.blue();
    int newA = newColor.alpha();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            uchar* pixel = imageData + y * bytesPerLine + x * 4;

            int pixelR = pixel[0];
            int pixelG = pixel[1];
            int pixelB = pixel[2];
            int pixelA = pixel[3];

            if (pixelR < 60 && pixelG < 60 && pixelB < 60 && pixelA > 0) {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
                pixel[3] = 255;
            } else if (pixelA > 0) {
                pixel[0] = newR;
                pixel[1] = newG;
                pixel[2] = newB;
                pixel[3] = newA;
            }
        }
    }

    setPixmap(QPixmap::fromImage(image));
}

void MapLayer::setFogOfWar(const QColor &newColor) {
    QImage fogTexture(":resources/Images/FogOfWar.png");

    if (fogTexture.isNull()) {
        qWarning() << "Fog texture is invalid or could not be loaded!";
        return;
    }

    QImage image = pixmap().toImage();
    QImage scaledFogTexture = fogTexture.scaled(image.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    int width = image.width();
    int height = image.height();

    QImage resultImage(image.size(), QImage::Format_RGBA8888);
    resultImage.fill(Qt::transparent);
    uchar* imageData = image.bits();
    uchar* resultData = resultImage.bits();
    uchar* fogData = scaledFogTexture.bits();
    int bytesPerLine = image.bytesPerLine();
    int fogBytesPerLine = scaledFogTexture.bytesPerLine();

    int newColorR = newColor.red();
    int newColorG = newColor.green();
    int newColorB = newColor.blue();
    int newColorA = newColor.alpha();

    unsigned char uNewColorR = static_cast<unsigned char>(newColorR);
    unsigned char uNewColorG = static_cast<unsigned char>(newColorG);
    unsigned char uNewColorB = static_cast<unsigned char>(newColorB);
    unsigned char uNewColorA = static_cast<unsigned char>(newColorA);

    for (int y = 0; y < height; ++y) {
        uchar* imageLine = imageData + y * bytesPerLine;
        uchar* resultLine = resultData + y * bytesPerLine;
        uchar* fogLine = fogData + y * fogBytesPerLine;

        for (int x = 0; x < width; ++x) {
            uchar* pixel = imageLine + x * 4;
            uchar* fogPixel = fogLine + x * 4;
            uchar* resultPixel = resultLine + x * 4;

            int bgA = pixel[3];
            int bgR = pixel[0];
            int bgG = pixel[1];
            int bgB = pixel[2];

            if (bgA == 0) continue;

            bgR = newColorR;
            bgG = newColorG;
            bgB = newColorB;

            int fogA = fogPixel[3];
            if (fogA > 0) {
                int alpha = fogA;
                int invAlpha = 255 - alpha;

                resultPixel[0] = (bgR * invAlpha + fogPixel[0] * alpha) >> 8;
                resultPixel[1] = (bgG * invAlpha + fogPixel[1] * alpha) >> 8;
                resultPixel[2] = (bgB * invAlpha + fogPixel[2] * alpha) >> 8;
                resultPixel[3] = 255;
            } else {
                resultPixel[0] = uNewColorR;
                resultPixel[1] = uNewColorG;
                resultPixel[2] = uNewColorB;
                resultPixel[3] = uNewColorA;
            }
        }
    }

    setPixmap(QPixmap::fromImage(resultImage));
}

void MapLayer::setArmyColor(ArmyType armyType) {
    switch (armyType) {
    case ArmyType::HAJDUK:
        ArmyColor = QColor(198,54,60,255);
        FogColor = QColor(204, 86, 91, 245);
        break;
    case ArmyType::JANISSARY:
        ArmyColor = QColor(0,149,48,255);
        FogColor = QColor(44, 153, 79, 245);
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
    if (isMainMode) {
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
}
void MapLayer::setTroopTextVisible(bool visible) {
    if (troopText) {
        troopText->setVisible(visible);
    }
}
void MapLayer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (isMainMode) {
        if(troopText->isVisible())
            setColor(ArmyColor);
        else
            setFogOfWar(FogColor);
    }
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void MapLayer::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit layerClicked(this);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

QColor MapLayer::getFogColor() const
{
    return FogColor;
}
void MapLayer::setMainMode(bool mainMode) {
    isMainMode = mainMode;
}
void MapLayer::setCurrentPlayer(int PlayerId){
    currentPlayer = PlayerId;
}

int MapLayer::getId(){
    return id;
}


