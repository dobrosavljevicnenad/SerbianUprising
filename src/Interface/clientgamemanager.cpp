#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(Client* client, QGraphicsScene* scene,QObject* parent)
    : QObject(parent), client(client),scene(scene) // Pointer to the graphical scene for rendering
{
    int clientId = client->getId();
    qDebug() << "ClientGameManager created with ID:" << clientId;
}

void ClientGameManager::initializeGraphics() {
    // Initialize empty map with graphical elements
    MapLayer* baseLayer = new MapLayer(":/resources/Images/base.png", false);
    baseLayer->setZValue(-1);
    scene->addItem(baseLayer);
}

void ClientGameManager::setScene(MapScene *newScene) {
    scene = newScene;
}

int ClientGameManager::getId() {
    return client->getId();
}
/*void ClientGameManager::updateGraphicsFromServerState(const QJsonObject& serverState) {
    g.deserialize(serverState);

    for (auto& [layer, vertex] : g.getLayerToVertex()) {
        layer->setTroopCount(vertex->army.getSoldiers());
        layer->setArmyColor(vertex->army.armyType());
    }
}*/

/*void ClientGameManager::sendEndTurn() {
    client.sendData("END_TURN");
}*/

/*void ClientGameManager::sendAction(const Action& action) {
    client->sendAction(action);
}

bool ClientGameManager::connectToServer() {
    if (client->connectToServer("127.0.0.1", 12345)) {
        QObject::connect(client, &Client::dataReceived, this, [](const QString &data) {
            qDebug() << "Client 1 received:" << data;
        });

        client->sendData("Hello from Player 1!");

        return true;
    } else {
        qWarning() << "Player 1 failed to connect!";
        return false;
    }

}*/


/*void ClientGameManager::processClientData(const QString &data) {
    // Deserialize incoming game state from the server
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject gameState = jsonDoc.object();
f
    deserializeGameState(gameState);  // Update client-side game state
    updateGraphics();  // Refresh the graphical display
}

void ClientGameManager::updateGraphics() {
    // Redraw map layers, troop counts, etc.
}*/
