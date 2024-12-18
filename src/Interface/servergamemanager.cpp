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

    rootObj = doc.object();
    graph->deserialize(rootObj);

}

void ServerGameManager::startGame() {
    qDebug() << "Starting game...";
    initializeGame();
    QJsonObject serialized_graph = graph->serialize(rootObj);
    //graph->print_graph();
    //graph->print_graph();
    emit serializedGraphReady1(serialized_graph);
}

void ServerGameManager::executeActions(const std::vector<Action> &actions1, int p1_id, const std::vector<Action> &actions2, int p2_id){

    for (const Action& action : actions1) {
        try {
            turn.addAction(p1_id, action); // Add the updated action
        } catch (const std::exception& e) {
            std::cerr << "Error adding action: " << e.what() << "\n";
        }
    }

    for (const Action& action : actions2) {
        try {
            turn.addAction(p2_id, action); // Add the updated action
        } catch (const std::exception& e) {
            std::cerr << "Error adding action: " << e.what() << "\n";
        }
    }
    turn.battlesResults.clear();
    turn.executeTurn();
    //graph->print_graph();
    QJsonObject serialized_graph = graph->serialize(rootObj);
    auto battleResults =turn.battlesResults;
    QJsonObject Results = turn.serializeResultsVector(battleResults);
    //graph->print_graph();
    emit serializedGraphReady2(serialized_graph,Results);
}

/*void ServerGameManager::processEndTurn() {
    // Example: Execute turn logic, update graph state
    turn.executeTurn();

    // Serialize and broadcast updated state to clients
    sendSerializedGameStateToClients();
}*/
