#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QGraphicsView> //*
#include "gui/mapscene.h" //*
#include "graph/graph.hpp"
#include "gui/maplayer.h"

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

    void print_connections(const graph::Graph &g, const graph::Vertex &vertex);


private:
    Ui::MainWindow *ui;
    MapScene *scene; //*
    QGraphicsView *view; //*
};
#endif // MAINWINDOW_H
