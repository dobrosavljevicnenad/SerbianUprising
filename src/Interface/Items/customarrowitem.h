#pragma once

#ifndef CUSTOMARROWITEM_H
#define CUSTOMARROWITEM_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>
#include <QGraphicsScene>

class CustomArrowItem : public QGraphicsLineItem {
public:
    CustomArrowItem(const QLineF& line, QGraphicsItem* parent = nullptr);

    QPainterPath shape() const override ;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void setNumber(int number);

private:
    QGraphicsTextItem* textItem;
};

#endif // CUSTOMARROWITEM_H
