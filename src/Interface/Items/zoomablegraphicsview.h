#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QTimer>


class ZoomableGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ZoomableGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QTimer* movementTimer;
    int horizontalDirection;
    int verticalDirection;

private slots:
    void moveView();
};

#endif // ZOOMABLEGRAPHICSVIEW_H
