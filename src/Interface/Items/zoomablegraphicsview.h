#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QTimer>
#include <QMouseEvent>
#include <QScrollBar>


class ZoomableGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ZoomableGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QTimer* movementTimer;
    QTimer* edgeScrollTimer;
    QPoint cursorPos;
    int edgeScrollSpeed;
    int horizontalDirection;
    int verticalDirection;

private slots:
    void moveView();
    void scrollOnEdge();
};

#endif // ZOOMABLEGRAPHICSVIEW_H
