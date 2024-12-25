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
#include "./Interface/infowidget.h"
#include "Interface/Items/zoomablegraphicsview.h"

#include <string>
class ClientWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ClientWindow(ClientGameManager *existingGameManager,QWidget *parent = nullptr);
    void setupGame();
    ~ClientWindow();

    void processEndTurnClicked();

    void checkServerClosed();
    void freezeUI();
    void showDisconnectPauseMenu();


protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    MapScene *scene;
    ZoomableGraphicsView *view;
    ClientGameManager *gameManager;
    QLabel *headerLabel;
    QLabel *yearDisplayLabel;
    QListWidget *moveList;
    QPushButton *moveButton;
    QPushButton *armyButton;
    QPushButton *infoButton;
    QPushButton *endTurnButton;
    QPushButton *activeButton;
    NodeInfoWidget* nodeInfoWidget;
    MapLayer *selectedLayer = nullptr;
    QWidget* layoutContainer;
    QWidget* mapModeContainer;
    QPushButton* reliefButton;
    QPushButton* regionsButton;
    QPushButton* cityButton;
    QPushButton* defaultButton;
    QPushButton* cultureButton;

    //QMediaPlayer* mediaPlayer;
    //QAudioOutput* audioOutput;

    void setupUI();
    void connectSignals();
    void showPauseMenu();
    void repositionFixedWidgets();
    void setupFixedWidgets();

    bool isPauseMenuActive = false;


private slots:
    void onMoveClicked(QListWidgetItem *item);
    void onEndTurnClicked();
    void clearExplosions();
    void onLayerClicked(MapLayer *layer);
    void handleMoveArmy(MapLayer* layer);
    void handlePlaceArmy(MapLayer* layer);
    void onInfoButtonClicked();
    void onMoveButtonClicked();
    void onPlaceButtonClicked();
    void setActiveButton(QPushButton* clickedButton);
    void updateYearLabel(QString year);
    void handleInfomation(MapLayer* layer);

signals:
    void endTurn_signal(const QVector<Action>& actions, int id);
};

#endif // CLIENTWINDOW_H
