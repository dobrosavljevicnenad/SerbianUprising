#include "servergamemanager.h"

ServerGameManager::ServerGameManager(Server* server, QObject* parent)
    : g(), // Initializes the graph
    turn(g),
    server(server),
    QObject(parent)
{
    // Initialize army managers or other server-specific data structures
    armyManagers[1] = AddArmyManager();
    armyManagers[2] = AddArmyManager();
}

void ServerGameManager::initializeMap() {
    // Same as your current `initializeMap` but skips graphical layer.
    // Example: Do not add `scene->addItem(...)`.
    QString filePath = "../../resources/init.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON file format.";
        return;
    }

    QJsonObject rootObj = doc.object();
    g.deserialize(rootObj);
}

/*void ServerGameManager::sendSerializedGameStateToClients() {
    QJsonObject serializedGraph = g.serialize();
    QString serializedData = QString(QJsonDocument(serializedGraph).toJson(QJsonDocument::Compact));

    server.sendData(serializedData);
}*/

/*void ServerGameManager::processEndTurn() {
    // Example: Execute turn logic, update graph state
    turn.executeTurn();

    // Serialize and broadcast updated state to clients
    sendSerializedGameStateToClients();
}*/

void ServerGameManager::startServer(){
    if (server->startServer(12345)) {
        QObject::connect(server, &Server::dataReceived, this, [this](const QString &data) {
        });
    }
}

/*void ServerGameManager::processServerData(const QString &data) {
    // Handle incoming data from clients
}*/

/*void ServerGameManager::onGameStart() {
    // Notify all connected clients to initialize their game states
    QJsonObject gameState = serializeGameState();  // Serialize game state
    QString serializedData = QJsonDocument(gameState).toJson();
    server.sendData(serializedData);  // Send game state to clients
}*/

