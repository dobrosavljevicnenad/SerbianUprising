#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Interface/mapscene.h"
#include "Interface/maplayer.h"
#include "Interface/gamemanager.h"
#include "base/Mechanics/turn.h"


#include <QMainWindow>
#include <QGraphicsView>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsProxyWidget>


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
    void onChangePlayerClicked();
    void onEndTurnClicked();

private:
    Ui::MainWindow *ui;
    MapScene *scene; //*
    QGraphicsView *view; //*
    MapLayer *selectedLayer = nullptr;
    GameManager* gameManager;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;


};
#endif // MAINWINDOW_H
