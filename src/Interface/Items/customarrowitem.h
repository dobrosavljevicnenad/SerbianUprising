#pragma once

#ifndef CUSTOMARROWITEM_H
#define CUSTOMARROWITEM_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPixmap>
#include <QColor>

class CustomArrowItem : public QGraphicsLineItem {
public:
    CustomArrowItem(const QPointF& from, const QPointF& to, int actionId, QColor color, QGraphicsItem* parent = nullptr);

    int getActionId() const;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QPixmap colorizePixmap(const QPixmap& pixmap, const QColor& color);

    int actionId;
    QPixmap arrowPixmap;
    QColor arrowColor;
};

#endif // CUSTOMARROWITEM_H
