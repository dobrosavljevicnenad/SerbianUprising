#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "server.h"
#include "client.h"
#include "../Interface/clientgamemanager.h"
#include "../Interface/servergamemanager.h"
#include "../clientwindow.h"

class ConnectionManager : public QObject {
    Q_OBJECT

public:
    explicit ConnectionManager(QObject *parent = nullptr);

    // Core initialization methods
    bool initializeServer();                // Set up the server
    bool initializeClient();                // Set up the client
    ServerGameManager* getServerManager();  // Get the initialized ServerGameManager
    ClientGameManager* getClientManager();  // Get the initialized ClientGameManager

signals:
    void gameStarted();                     // Emitted when the game starts

private:
    Server server;                          // Server instance
    Client* client;                          // Client instance
    ServerGameManager *serverManager;       // Server-side manager
    ClientGameManager *clientManager;       // Client-side manager
};
#endif // CONNECTION_H
