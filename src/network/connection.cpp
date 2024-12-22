#include "connection.h"


ConnectionManager::ConnectionManager(QObject *parent)
: QObject(parent), clientManager(nullptr),client(new Client(this)) {
}

bool ConnectionManager::initializeServer() {
    qDebug() << "Player";
    if (!server.startServer(12345)) {
        qWarning() << "Failed to start the server on port 12345.";
        return false;
    }
    qDebug() << "Server initialized.";
    return true;
}

bool ConnectionManager::initializeClient() {
    if (!client->connectToServer("192.168.1.7", 12345)) {
        qWarning() << "Failed to connect the client to the server.";
        return false;
    }

    connect(client, &Client::idReceived, this, [this](int id) {
        qDebug() << "Client received ID from server:" << id;

        clientManager = client->getClientGameManager();
    });

    connect(client, &Client::gameStarted, this, &ConnectionManager::gameStarted);
    std::cout << "POVEZANA OBA KLIJENTA" << std::endl;
    return true;
}


ClientGameManager* ConnectionManager::getClientManager() {
    return clientManager;
}

