#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(Client* client, QGraphicsScene* scene,QObject* parent)
    : QObject(parent), client(client),scene(scene) // Pointer to the graphical scene for rendering
{
    int clientId = client->getId();
    qDebug() << "ClientGameManager created with ID:" << clientId;
}

void ClientGameManager::initializeGraphics() {
    // File path to JSON containing graphical data
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

    // Base layer initialization
    MapLayer* baseLayer = new MapLayer(":/resources/Images/base.png", false);
    baseLayer->setZValue(-1);
    scene->addItem(baseLayer);

    // Load and initialize layers from JSON
    QJsonArray layersArray = rootObj["layers"].toArray();

    std::vector<MapLayer*> layers(layersArray.size());
    for (int i = 0; i < layersArray.size(); ++i) {
        QJsonObject layerObj = layersArray[i].toObject();

        // Extract layer-specific data
        QString labelPath = layerObj.value("label_path").toString();
        QJsonObject positionObj = layerObj.value("position").toObject();
        int posX = positionObj.value("x").toInt();
        int posY = positionObj.value("y").toInt();

        // Initialize the graphical layer
        MapLayer* layer = new MapLayer(labelPath, true);
        layer->setZValue(0); // Default Z-value for layers
        layer->setPos(posX, posY);

        // Add layer to the scene
        scene->addItem(layer);

        // Store layer in the list for future reference
        layers[i] = layer;
    }

    // Store all layers
    this->layers = layers;

    qDebug() << "Graphics initialized with" << layers.size() << "layers.";
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
