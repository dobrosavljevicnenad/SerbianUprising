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

private:
    void setupUI();
    void loadSavedGames();
    void setupConnections();
    void setBackgroundImage();


    QLabel *player1ArmyLabel;
    QLabel *player2ArmyLabel;

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

signals:
    void backToLobby();

private slots:
    void updateArmySelection(const QString &playerArmy);
    void handleGameStart();
    void onFileClicked(int row, int column);
    void onLoadGameClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

};
