#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include "network/server.h"
#include "network/client.h"
#include "Interface/servergamemanager.h"
#include "Interface/clientgamemanager.h"
#include "network/connection.h"


class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

    QPushButton* getCreateLobbyButton() const;


private:
    ConnectionManager *connectionManager;
    ClientWindow *gameWindow;

    ServerGameManager* serverManager;
    ClientGameManager* clientManager;

    QFrame *buttonFrame;

    QPushButton *createLobbyButton;
    QPushButton *joinLobbyButton;
    QPushButton *backButton;

    ArmyType ARMY1;
    ArmyType ARMY2;

    void setupUI();
    void connectSignals();
    void setBackgroundImage();



private slots:
    void onCreateServer();
    void onJoinGame();
    void returnToMenu();

signals:
    void openCreateLobby();
    void backToMenu();
    void startGame();
    void createLobby();
    void stopBackgroundMusic();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

