#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(QGraphicsScene* scene,QObject* parent)
    : QObject(parent),scene(scene) // Pointer to the graphical scene for rendering
{
    qDebug() << "ClientGameManager created with ID:" << ClientId;
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
    QJsonArray layersArray = rootObj["vertices"].toArray();

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
        std::string type = layerObj.value("army_type").toString().toStdString();
        if (type == "HAJDUK") {
            layer->setArmyColor(ArmyType::HAJDUK);
        } else if (type == "JANISSARY") {
            layer->setArmyColor(ArmyType::JANISSARY);
        }
        layer->setCurrentPlayer(ClientId);
        layer->setZValue(0); // Default Z-value for layers
        layer->setPos(posX, posY);

        // Add layer to the scene
        scene->addItem(layer);

        // Store layer in the list for future reference
        layers[i] = layer;

        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });
    }

    // Store all layers
    this->layers = layers;
}

void ClientGameManager::setScene(MapScene *newScene) {
    scene = newScene;
}

void ClientGameManager::setId(int id) {
    ClientId = id;
}

void ClientGameManager::printConnections() {
    g.print_graph();
}

void ClientGameManager::processDataFromServer(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject graphData = doc.object();

        g.deserialize(graphData);

        qDebug() << "Graph data deserialized and processed.";
    } else {
        qWarning() << "Invalid data received from server.";
    }
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
*/
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
