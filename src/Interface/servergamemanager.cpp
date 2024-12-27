#include "servergamemanager.h"

ServerGameManager::ServerGameManager( QObject* parent)
    : graph(new graph::Graph()),
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

    if (rootObj.contains("events") && rootObj["events"].isArray()) {
        QJsonArray eventsArray = rootObj["events"].toArray();
        eventHandle.deserializeEvents(eventsArray, 0);
    }
}

void ServerGameManager::startGame() {
    qDebug() << "Starting game...";
    initializeGame();
    QJsonObject serialized_graph = graph->serialize(rootObj);
    //graph->print_graph();
    emit init_serializedGraphReady(serialized_graph);
}

void ServerGameManager::executeActions(const std::vector<Action> &actions1, int p1_id, const std::vector<Action> &actions2, int p2_id){
    gameYear.advanceThreeMonths();
    for (const Action& action : actions1) {
        try {
            turn.addAction(p1_id, action);
        } catch (const std::exception& e) {
            std::cerr << "Error adding action: " << e.what() << "\n";
        }
    }

    for (const Action& action : actions2) {
        try {
            turn.addAction(p2_id, action);
        } catch (const std::exception& e) {
            std::cerr << "Error adding action: " << e.what() << "\n";
        }
    }
    turn.battlesResults.clear();
    turn.executeTurn();
    eventHandle.processEvents(gameYear.toJsonDateString(), *graph);
    turn.executeTurn();

    QJsonObject serialized_graph = graph->serialize(rootObj);

    auto battleResults =turn.battlesResults;
    QJsonObject Results = turn.serializeResultsVector(battleResults);

    emit serializedGraphReady2(serialized_graph,Results);
}

void ServerGameManager::processLoadGame(QJsonObject& gameData){
    graph->deserialize(gameData);
    QJsonObject serializedGraph = graph->serialize(QJsonObject());
    emit loadGame_serializedGraphReady(serializedGraph);
}
