#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Interface/mapscene.h"
#include "Interface/maplayer.h"
#include "Interface/gamemanager.h"
#include "base/Mechanics/turn.h"
#include "base/Mechanics/addarmymanager.h"


#include <QMainWindow>
#include <QGraphicsView>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsProxyWidget>
#include <QListWidget>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>

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

    void zoomOutView(QGraphicsView* view, qreal zoomFactor = 1.75);
    void highlightLayer(MapLayer *layer);

    void print_connections(const graph::Graph &g, const graph::Vertex* vertex);

private slots:
    void onLayerClicked(MapLayer *layer);
    void onMoveClicked(QListWidgetItem* item);
    void onChangePlayerClicked();
    void onEndTurnClicked();
    void updateMoveList(int currentPlayer);

    void onInfoButtonClicked();
    void onMoveButtonClicked();
    void onPlaceButtonClicked();
    void setActiveButton(QPushButton* clickedButton);
    void handleMoveArmy(MapLayer* layer);
    void handlePlaceArmy(MapLayer* layer);

private:
    Ui::MainWindow *ui;
    MapScene *scene; //*
    QGraphicsView *view; //*
    MapLayer *selectedLayer = nullptr;
    GameManager* gameManager;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
    QListWidget* moveList;
    QPushButton* moveButton;
    QPushButton* armyButton;
    QPushButton* activeButton ;
    QLabel* headerLabel;
};
#endif // MAINWINDOW_H
