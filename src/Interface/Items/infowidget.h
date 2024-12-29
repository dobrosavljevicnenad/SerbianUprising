#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <qpushbutton.h>
#include "../../base/graph/vertex.hpp"

class NodeInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit NodeInfoWidget(QMap<MapLayer*,graph::Vertex*> layerToVertex, QWidget* parent = nullptr);

    void updateNodeInfo(MapLayer* layer);
    void updateLayerToVertex(const QMap<MapLayer*, graph::Vertex*>& newLayerToVertex);


private:
    QMap<MapLayer*,graph::Vertex*> layerToVertex;
    QLabel* titleLabel;
    QLabel* imageLabel;
    QLabel* backgroundImageLabel;
    QLabel* troopCountLabel;
    QLabel* cultureLabel;
    QLabel* neighborsLabel;
    QLabel* bioLabel;
    QLabel* cityLabel;
    QLabel* regionLabel;
    QPushButton* closeButton;
    QLabel* bioTextLabel;
};

#endif // INFOWIDGET_H
