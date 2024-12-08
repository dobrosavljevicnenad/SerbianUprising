#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include "Interface/mapscene.h"
#include "Interface/clientgamemanager.h"
#include "base/Mechanics/Action.h"
#include "base/Mechanics/addarmymanager.h"

class ClientWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ClientWindow(ClientGameManager *existingGameManager,QWidget *parent = nullptr);
    void setupGame();
    ~ClientWindow();


private:
    MapScene *scene;
    QGraphicsView *view;
    ClientGameManager *gameManager;
    QLabel *headerLabel;
    // UI elements specific to the client
    QListWidget *moveList;
    QPushButton *moveButton;
    QPushButton *armyButton;
    QPushButton *infoButton;
    QPushButton *endTurnButton;
    QPushButton *activeButton;

    MapLayer *selectedLayer = nullptr;
    //QMediaPlayer* mediaPlayer;
    //QAudioOutput* audioOutput;

    void setupUI();
    void connectSignals();

private slots:
    void onMoveClicked(QListWidgetItem *item);
    void onEndTurnClicked();

    void onLayerClicked(MapLayer *layer);
    void handleMoveArmy(MapLayer* layer);
    void handlePlaceArmy(MapLayer* layer);

    //void updateMoveList(int currentPlayer);


    void onInfoButtonClicked();
    void onMoveButtonClicked();
    void onPlaceButtonClicked();
    void setActiveButton(QPushButton* clickedButton);

signals:
    void endTurn_signal(const QVector<Action>& actions, int id);
};

#endif // CLIENTWINDOW_H
