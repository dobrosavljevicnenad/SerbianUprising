#include "servergamemanager.h"

ServerGameManager::ServerGameManager( QObject* parent)
    : graph(new graph::Graph()), // Initializes the graph
    turn(*graph),
    QObject(parent)
{
}

void ServerGameManager::initializeGame() {
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
    graph->deserialize(rootObj);

}

void ServerGameManager::startGame() {
    qDebug() << "Starting game...";
    initializeGame();
    QJsonObject serialized_graph = graph->serialize();
    //g.deserialize(serialized_graph);
    //.print_graph();
    emit serializedGraphReady(serialized_graph);
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

