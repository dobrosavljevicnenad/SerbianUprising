#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(QGraphicsScene* scene,QObject* parent)
    : QObject(parent),scene(scene),clientGraph(new graph::Graph())
{
}

void ClientGameManager::initializeUI(
    QLabel* headerLabel, QPushButton* endTurnButton, QPushButton* moveButton, QPushButton* infoButton, QListWidget* moveList,QPushButton* armyButton) {
    this->headerLabel = headerLabel;
    this->endTurnButton = endTurnButton;
    this->moveButton = moveButton;
    this->infoButton = infoButton;
    this->moveList = moveList;
    this->armyButton = armyButton;
}

void ClientGameManager::initializeGraphics() {
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

    MapLayer* baseLayer = new MapLayer(20,":/resources/Images/base.png", false);
    baseLayer->setZValue(-1);
    scene->addItem(baseLayer);

    QJsonArray layersArray = rootObj["vertices"].toArray();

    std::vector<MapLayer*> layers(layersArray.size());
    for (int i = 0; i < layersArray.size(); ++i) {
        QJsonObject layerObj = layersArray[i].toObject();

        QString labelPath = layerObj.value("label_path").toString();
        QJsonObject positionObj = layerObj.value("position").toObject();
        int posX = positionObj.value("x").toInt();
        int posY = positionObj.value("y").toInt();

        MapLayer* layer = new MapLayer(i,labelPath, true);
        layer->setCurrentPlayer(ClientId);
        layer->setZValue(0);
        layer->setPos(posX, posY);
        scene->addItem(layer);

        layers[i] = layer;
        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });
    }

    this->layers = layers;
}

void ClientGameManager::setScene(MapScene *newScene) {
    scene = newScene;
}

void ClientGameManager::setId(int id) {
    ClientId = id;
    player = Player();
    player.setPlayerId(ClientId);
    ClientId == 1 ? player.setArmyType(ArmyType::HAJDUK) : player.setArmyType(ArmyType::JANISSARY);
}

void ClientGameManager::printConnections() {
    clientGraph->print_graph();
}

void ClientGameManager::processDataFromServer(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject graphData = doc.object();
        clientGraph->deserialize(graphData);
        if(init){
            allReset();
            TurnId = TurnId + 1;
        } else if(!init){
            for (auto &layer : layers) {
                graph::Vertex* vertex = clientGraph->get_vertex_by_id(layer->getId()+1);
                if (vertex) {
                    layerToVertex[layer] = vertex;
                    vertex->map_layer = layer;
                }
            }
            armyManager = AddArmyManager();
            init = true;
        }
        enableInteractions();
        updateGraphics();
    } else {
        qWarning() << "Invalid data received from server.";
    }
}

void ClientGameManager::disableInteractions() {
    headerLabel->setText("Waiting for new turn...");

    for (auto &layer : layers) {
        layer->setEnabled(false);
    }
    if (moveButton) moveButton->setEnabled(false);
    if (infoButton) infoButton->setEnabled(false);
    if (endTurnButton) endTurnButton->setEnabled(false);
    if (moveList) moveList->setEnabled(false);
    if (armyButton) armyButton->setEnabled(false);
}

void ClientGameManager::enableInteractions() {
    moveList->clear();
    moveList->addItem(QString("Player %1 on turn:").arg(ClientId));
    headerLabel->setText(QString("Turn %1").arg(TurnId));

    for (auto &layer : layers) {
        layer->setEnabled(true);
    }
    if (moveButton) moveButton->setEnabled(true);
    if (infoButton) infoButton->setEnabled(true);
    if (endTurnButton) endTurnButton->setEnabled(true);
    if (moveList) moveList->setEnabled(true);
    if (armyButton) armyButton->setEnabled(true);
}

void ClientGameManager::allReset(){
    armyManager.endTurn();
    clearArrows();
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

AddArmyManager& ClientGameManager::getArmyManager() {
    return armyManager;
}

void ClientGameManager::removeActionById(int actionId) {
    auto& buffer = actionBuffer;

    auto it = std::find_if(buffer.begin(), buffer.end(),
                           [actionId](const Action& action) {
                               return action.id == actionId;
                           });

    if (it != buffer.end()) {
        graph::Vertex* cvor = clientGraph->get_vertex_by_id(it->sourceVertexId);
        cvor->army.setSoldiers(cvor->army.getSoldiers() + it->soldiers);
        cvor->map_layer->setTroopCount(cvor->army.getSoldiers());
        buffer.erase(it);
        std::cout << "Action with ID " << actionId << " removed for Player " << ClientId << ".\n";
    } else {
        std::cerr << "Action with ID " << actionId << " not found for Player " << ClientId << ".\n";
    }
}

void ClientGameManager::removeArrowByActionId(int actionId) {
    for (auto& [playerId, arrowList] : arrows) {
        for (auto it = arrowList.begin(); it != arrowList.end(); ++it) {
            if ((*it)->getActionId() == actionId) {
                scene->removeItem(*it);
                delete *it;
                arrowList.erase(it);
                return;
            }
        }
    }
}
QString ClientGameManager::GetCurrentAction(const Action& action) {
    QString moveDescription = QString("%2 troops from Layer %3 to Layer %4")
    .arg(action.soldiers)
        .arg(action.sourceVertexId)
        .arg(action.targetVertexId);
    return moveDescription;
}

void ClientGameManager::EndTurnClicked(const QVector<Action>& actions, int id){
    emit endTurnActionsReady(actions, id);
}

void ClientGameManager::clearArrows() {
    for (auto& [playerId, arrowList] : arrows) {
        for (CustomArrowItem* arrow : arrowList) {
            scene->removeItem(arrow);
        }
    }
}
