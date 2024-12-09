#include "zoomablegraphicsview.h"

ZoomableGraphicsView::ZoomableGraphicsView(QWidget* parent)
    : QGraphicsView(parent), horizontalDirection(0), verticalDirection(0) {
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &ZoomableGraphicsView::moveView);
    movementTimer->start(30); // Adjust for smoothness
}


void ZoomableGraphicsView::wheelEvent(QWheelEvent *event){
    constexpr double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor); // Zoom in
    }
    else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out

    }
}

void ZoomableGraphicsView::keyPressEvent(QKeyEvent* event) {
    constexpr int moveStep = 20; // Movement step size

    switch (event->key()) {
    case Qt::Key_Left:
        horizontalDirection = -moveStep;
        break;
    case Qt::Key_Right:
        horizontalDirection = moveStep;
        break;
    case Qt::Key_Up:
        verticalDirection = -moveStep;
        break;
    case Qt::Key_Down:
        verticalDirection = moveStep;
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void ZoomableGraphicsView::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
        horizontalDirection = 0;
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
        verticalDirection = 0;
        break;
    default:
        QGraphicsView::keyReleaseEvent(event);
        break;
    }
}

void ZoomableGraphicsView::moveView() {
    if (horizontalDirection != 0) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + horizontalDirection);
    }
    if (verticalDirection != 0) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + verticalDirection);
    }
}
