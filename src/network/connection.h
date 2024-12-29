#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "server.h"
#include "client.h"
#include "../Interface/clientgamemanager.h"
#include "../Interface/servergamemanager.h"
#include "../clientwindow.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <iostream>

class ConnectionManager : public QObject {
    Q_OBJECT

public:
    explicit ConnectionManager(QObject *parent = nullptr);

    bool initializeServer(int clientId);
    bool initializeClient();
    ClientGameManager* getClientManager();
    QString getLocalIpAddress();

    QString ipAddress = "";


signals:
    void gameStarted();

private:
    Server server;
    Client* client;
    ClientGameManager *clientManager;
};
#endif // CONNECTION_H
