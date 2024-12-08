#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(QGraphicsScene* scene,QObject* parent)
    : QObject(parent),scene(scene),clientGraph(new graph::Graph()) // Pointer to the graphical scene for rendering
{
    qDebug() << "ClientGameManager created with ID:" << ClientId;
}

void ClientGameManager::initializeGraphics() {
    // File path to JSON containing graphical data
    //qDebug() << "prvo init:";
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
    MapLayer* baseLayer = new MapLayer(20,":/resources/Images/base.png", false);
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
        MapLayer* layer = new MapLayer(i,labelPath, true);
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
    clientGraph->print_graph();
}

void ClientGameManager::processDataFromServer(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject graphData = doc.object();
        clientGraph->deserialize(graphData);
        if(!init){
            for (auto &layer : layers) {
                graph::Vertex* vertex = clientGraph->get_vertex_by_id(layer->getId()+1);
                if (vertex) {
                    layerToVertex[layer] = vertex;
                    vertex->map_layer = layer;
                }
            }
            init = true;
        }
        updateGraphics();
    } else {
        qWarning() << "Invalid data received from server.";
    }
}

void ClientGameManager::updateGraphics() {
    for (auto &layer : layers) {
        if (layerToVertex.find(layer) != layerToVertex.end()) {
            graph::Vertex *vertex = layerToVertex[layer];
            if (vertex) {
                Army army = vertex->army; // Safely access the `army`
                layer->setArmyColor(army.armyType());
                layer->setTroopCount(army.getSoldiers());
            } else {
                qWarning() << "Vertex is null for layer:" << layer->getId();
            }
        } else {
            qWarning() << "Layer not found in layerToVertex map:" << layer->getId();
        }
    }
}

void ClientGameManager::drawArrow(int playerId, MapLayer* from, MapLayer* to, int number, int actionId) {
    QPointF fromPos = from->pos() + QPointF((from->boundingRect().width() / 2)-5,
                                            from->boundingRect().height() / 2);
    QPointF toPos = to->pos() + QPointF((to->boundingRect().width() / 2)+20,
                                        to->boundingRect().height() / 2);

    QLineF line(fromPos, toPos);
    CustomArrowItem* arrow = new CustomArrowItem(line,actionId);
    scene->addItem(arrow);
    arrow->setNumber(number);
    arrows[playerId].emplace_back(arrow);
}

void ClientGameManager::addAction(const Action& action){
    actionBuffer.push_back(action);
}

QString ClientGameManager::GetCurrentAction(const Action& action) {
    QString moveDescription = QString("%2 troops from Layer %3 to Layer %4")
    .arg(action.soldiers)
        .arg(action.sourceVertexId)
        .arg(action.targetVertexId);
    return moveDescription;
}

AddArmyManager& ClientGameManager::getArmyManager(int playerId) {
    auto it = armyManagers.find(playerId);
    if (it != armyManagers.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid player ID for ArmyManager");
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
}*/

