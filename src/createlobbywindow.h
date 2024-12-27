#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QDir>
#include "network/connection.h"
#include "clientwindow.h"
#include "Interface/servergamemanager.h"
#include "Interface/clientgamemanager.h"


class CreateLobbyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateLobbyWindow(QWidget *parent = nullptr);
    ~CreateLobbyWindow();

    void handleGameStart();

private:
    void setupUI();
    void loadSavedGames();
    void setupConnections();
    void setBackgroundImage();
    void setupMusic();

    QLabel *player1ArmyLabel;
    QLabel *player2ArmyLabel;
    QLabel *ip;

    QTableWidget *savedGamesTable;
    QComboBox *armyComboBox;
    QPushButton *loadButton;
    QPushButton *startButton;
    QPushButton *exitButton;

    ServerGameManager* serverManager;
    ClientGameManager* clientManager;

    ConnectionManager *connectionManager;
    ClientWindow *gameWindow;

    QString selectedFile = nullptr;

    ArmyType ARMY1;
    ArmyType ARMY2;

    bool isFileClicked = false;
    bool isGameLoaded = false;
    bool isArmySelected = false;
    bool isGameStarted = false;

signals:
    void backToLobby();

private slots:
    void updateArmySelection(const QString &playerArmy);
    void onFileClicked(int row, int column);
    void onLoadGameClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

};
