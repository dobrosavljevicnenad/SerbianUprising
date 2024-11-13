#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "Interface/mapscene.h"
#include "base/graph/graph.hpp"
#include "Interface/maplayer.h"
#include "Interface/gamemanager.h"
#include <QInputDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void highlightLayer(MapLayer *layer);

    void print_connections(const graph::Graph &g, const graph::Vertex* vertex);

private slots:
    void onLayerClicked(MapLayer *layer);
private:
    Ui::MainWindow *ui;
    MapScene *scene; //*
    QGraphicsView *view; //*
    MapLayer *selectedLayer = nullptr;
};
#endif // MAINWINDOW_H
