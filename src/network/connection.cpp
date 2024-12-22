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
    QString localIp = getLocalIpAddress();
    std::cout << localIp.toStdString() << std::endl;
    if (localIp.isEmpty()) {
        qWarning() << "Could not determine local IP address.";
        return false;
    }

    qDebug() << "Local IP address detected:" << localIp;

    if (!client->connectToServer(localIp, 12345)) {
        qWarning() << "Failed to connect the client to the server.";
        return false;
    }

    connect(client, &Client::idReceived, this, [this](int id) {
        qDebug() << "Client received ID from server:" << id;

        clientManager = client->getClientGameManager();
    });

    connect(client, &Client::gameStarted, this, &ConnectionManager::gameStarted);
    return true;
}

QString ConnectionManager::getLocalIpAddress() {
    const QList<QHostAddress>& allAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress& address : allAddresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
            return address.toString();
        }
    }
    return QString();
}


ClientGameManager* ConnectionManager::getClientManager() {
    return clientManager;
}

