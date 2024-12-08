#include "zoomablegraphicsview.h"

ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
}

void ZoomableGraphicsView::wheelEvent(QWheelEvent *event){
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);//zoomin
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);//zoomout
    }
}

void ZoomableGraphicsView::keyPressEvent(QKeyEvent *event) {
    const double scaleFactor = 1.15;
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
        scale(scaleFactor, scaleFactor);// ZOOOMIN
    } else if (event->key() == Qt::Key_Minus) {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);//zomout
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}
