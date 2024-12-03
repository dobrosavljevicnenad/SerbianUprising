#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "network/server.h"
#include "network/client.h"
#include "clientwindow.h"
#include "Interface/servergamemanager.h"
#include "Interface/clientgamemanager.h"


class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();


private:
    Server server;
    Client client;

    ServerGameManager* serverManager;
    ClientGameManager* clientManager;

    QPushButton *createServerButton;
    QPushButton *joinGameButton;
    QPushButton *backButton;

    void setupUI();
    void connectSignals();

private slots:
    void onCreateServer();
    void onJoinGame();
    void handleGameStarted();
};

#endif // LOBBYWINDOW_H
