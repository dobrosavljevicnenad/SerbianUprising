#include "customarrowitem.h"

CustomArrowItem::CustomArrowItem(const QPointF& from, const QPointF& to, int actionId, QColor color, QGraphicsItem* parent)
    : QGraphicsLineItem(QLineF(from, to), parent), actionId(actionId), arrowPixmap(":/resources/Images/arrow.png"), arrowColor(color) {
    if (arrowPixmap.isNull()) {
        qDebug() << "Failed to load arrow image!";
    } else {
        arrowPixmap = colorizePixmap(arrowPixmap, arrowColor);
    }
}

QPixmap CustomArrowItem::colorizePixmap(const QPixmap& pixmap, const QColor& color) {
    QImage image = pixmap.toImage();
    QPainter imagePainter(&image);
    imagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    imagePainter.fillRect(image.rect(), color);
    imagePainter.end();
    return QPixmap::fromImage(image);
}

void CustomArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setOpacity(0.4);

    QLineF line = this->line();
    double lineLength = line.length();
    double angle = std::atan2(-line.dy(), line.dx()) * 180.0 / M_PI;

    if (arrowPixmap.isNull()) {
        return;
    }

    QSizeF originalSize = arrowPixmap.size();
    double scaleFactor = lineLength / originalSize.width();
    double heightFactor = 0.5;
    QSizeF scaledSize(originalSize.width() * scaleFactor, originalSize.height() * scaleFactor * heightFactor);
    QRectF arrowRect(-scaledSize.width() / 2, -scaledSize.height() / 2, scaledSize.width(), scaledSize.height());

    painter->save();
    painter->translate(line.center());
    painter->rotate(-angle);
    painter->drawPixmap(arrowRect, arrowPixmap, arrowPixmap.rect());
    painter->restore();
}

int CustomArrowItem::getActionId() const {
    return actionId;
}
