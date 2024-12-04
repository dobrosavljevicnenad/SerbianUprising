    #include "connection.h"


    ConnectionManager::ConnectionManager(QObject *parent)
    : QObject(parent), serverManager(nullptr), clientManager(nullptr),client(new Client(this)) {
    }

    bool ConnectionManager::initializeServer() {
        qDebug() << "Player";
        if (!server.startServer(12345)) {
            qWarning() << "Failed to start the server on port 12345.";
            return false;
        }
        serverManager = new ServerGameManager(&server, this);
        qDebug() << "Server and ServerManager initialized.";
        return true;
    }

    bool ConnectionManager::initializeClient() {
        if (!client->connectToServer("127.0.0.1", 12345)) {
            qWarning() << "Failed to connect the client to the server.";
            return false;
        }

        connect(client, &Client::idReceived, this, [this](int id) {
            qDebug() << "Client received ID from server:" << id;

            clientManager = new ClientGameManager(client, nullptr, this);
        });

        connect(client, &Client::gameStarted, this, &ConnectionManager::gameStarted);

        return true;
    }

    ServerGameManager* ConnectionManager::getServerManager() {
        return serverManager;
    }

    ClientGameManager* ConnectionManager::getClientManager() {
        return clientManager;
    }

