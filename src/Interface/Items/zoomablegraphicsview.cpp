#include "zoomablegraphicsview.h"

ZoomableGraphicsView::ZoomableGraphicsView(QWidget* parent)
    : QGraphicsView(parent), horizontalDirection(0), verticalDirection(0), edgeScrollSpeed(50), edgeScrollTimer(new QTimer(this)) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);
    setMouseTracking(true);
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &ZoomableGraphicsView::moveView);
    movementTimer->start(30);
    connect(edgeScrollTimer, &QTimer::timeout, this, &ZoomableGraphicsView::scrollOnEdge);
    edgeScrollTimer->start(16);
}

void ZoomableGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    cursorPos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void ZoomableGraphicsView::scrollOnEdge() {
    if (!scene()) return;

    QRect viewRect = viewport()->rect();
    int cursorX = cursorPos.x();
    int cursorY = cursorPos.y();

    qreal scrollX = 0;
    qreal scrollY = 0;

    if (cursorX <= 30) {
        scrollX = -edgeScrollSpeed * (30 - cursorX) / 30.0;
    } else if (cursorX >= viewRect.width() - 30) {
        scrollX = edgeScrollSpeed * (cursorX - (viewRect.width() - 30)) / 30.0;
    }

    if (cursorY <= 30) {
        scrollY = -edgeScrollSpeed * (30 - cursorY) / 30.0;
    } else if (cursorY >= viewRect.height() - 30) {
        scrollY = edgeScrollSpeed * (cursorY - (viewRect.height() - 30)) / 30.0;
    }

    if (scrollX != 0 || scrollY != 0) {
        QPointF currentCenter = mapToScene(viewport()->rect().center());
        centerOn(currentCenter + QPointF(scrollX, scrollY));
    }
}

void ZoomableGraphicsView::wheelEvent(QWheelEvent *event) {
    constexpr double scaleFactor = 1.15;
    constexpr double maxZoomLevel = 5.0;
    constexpr double minZoomWidth = 3840;
    constexpr double minZoomHeight = 2160;

    QRectF visibleRect = mapToScene(viewport()->rect()).boundingRect();
    double currentWidth = visibleRect.width();
    double currentHeight = visibleRect.height();

    if (event->angleDelta().y() > 0) {
        if (transform().m11() < maxZoomLevel) {
            scale(scaleFactor, scaleFactor);
        }
    } else {
        double newWidth = currentWidth * scaleFactor;
        double newHeight = currentHeight * scaleFactor;

        if (newWidth <= minZoomWidth && newHeight <= minZoomHeight) {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        } else {
            fitInView(0, 0, minZoomWidth, minZoomHeight, Qt::KeepAspectRatio);
        }
    }
}

void ZoomableGraphicsView::keyPressEvent(QKeyEvent* event) {
    constexpr int moveStep = 40;

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
