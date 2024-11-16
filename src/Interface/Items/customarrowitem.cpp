#include "customarrowitem.h"

CustomArrowItem::CustomArrowItem(const QLineF& line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent),textItem(nullptr) {
    textItem = new QGraphicsTextItem(this);
    textItem->setDefaultTextColor(Qt::black);
    QFont font = textItem->font();
    font.setPointSize(13);
    textItem->setFont(font);

    QPointF midpoint = line.pointAt(0.5);
    textItem->setPos(midpoint);
}

QPainterPath CustomArrowItem::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();

    QPolygonF arrowHead;
    qreal arrowSize = 20;
    QLineF line = this->line();
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF arrowP1 = line.p2() - QPointF(std::cos(angle + M_PI / 6) * arrowSize,
                                          std::sin(angle + M_PI / 6) * arrowSize);
    QPointF arrowP2 = line.p2() - QPointF(std::cos(angle - M_PI / 6) * arrowSize,
                                          std::sin(angle - M_PI / 6) * arrowSize);

    arrowHead << line.p2() << arrowP1 << arrowP2;

    path.addPolygon(arrowHead);

    return path;
}

void CustomArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsLineItem::paint(painter, option, widget);

    QPolygonF arrowHead;
    qreal arrowSize = 15;
    QLineF line = this->line();
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF arrowP1 = line.p2() - QPointF(std::cos(angle + M_PI / 6) * arrowSize,
                                          std::sin(angle + M_PI / 6) * arrowSize);
    QPointF arrowP2 = line.p2() - QPointF(std::cos(angle - M_PI / 6) * arrowSize,
                                          std::sin(angle - M_PI / 6) * arrowSize);

    arrowHead << line.p2() << arrowP1 << arrowP2;

    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::yellow);
    painter->drawPolygon(arrowHead);
}

void CustomArrowItem::setNumber(int number) {
    if (textItem) {
        textItem->setPlainText(QString::number(number));
        QLineF line = this->line();
        QPointF midpoint = line.pointAt(0.5);
        textItem->setPos(midpoint);
    }
}
