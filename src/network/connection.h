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

    void finalizeSetup();                   // Finalize connections and transitions

signals:
    void clientsReady();                    // Emitted when both clients are ready
    void gameStarted();                     // Emitted when the game starts

private:
    Server server;                          // Server instance
    Client* client;                          // Client instance
    ServerGameManager *serverManager;       // Server-side manager
    ClientGameManager *clientManager;       // Client-side manager
    int connectedClients = 0;               // Counter for connected clients
    int id = 1;
};
#endif // CONNECTION_H
