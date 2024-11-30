#include "gamemanager.h"

GameManager::GameManager(QGraphicsScene* scene) : scene(scene), turn(g)  {}

void GameManager::initializeMap(){
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

    QJsonArray layersArray = rootObj["layers"].toArray();

    std::vector<MapLayer*> layers(layersArray.size());
    std::vector<Army> armies;
    Player player1(1,ArmyType::HAJDUK);
    Player player2(2,ArmyType::JANISSARY);
    std::vector<std::string> labels;
    std::vector<std::pair<int,int>>positions;
    std::vector<std::pair<int,std::vector<int>>> allNeighbors;

    MapLayer *baseLayer = new MapLayer(":/resources/Images/base.png", false);
    baseLayer->setZValue(-1);
    scene->addItem(baseLayer);

    Terrain defaultTerrain(TerrainType::MOUNTAIN);

    for (int i = 0; i <layersArray.size(); ++i) {
        QJsonObject layerObj = layersArray[i].toObject();

        std::string label = layerObj.value("label").toString().toStdString();
        labels.push_back(label);

        QString labelPath = layerObj.value("label_path").toString();
        std::string armyType = layerObj.value("army_type").toString().toStdString();
        int numOfSoldiers = layerObj.value("num_of_soldiers").toInt();

        layers[i] = (new MapLayer(labelPath, true));

        ArmyType type = (armyType == "HAJDUK") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        armies.emplace_back(numOfSoldiers,type);
        QJsonObject positionObj = layerObj.value("position").toObject();

        int posX = positionObj.value("x").toInt();
        int posY = positionObj.value("y").toInt();
        positions.push_back(std::make_pair(posX,posY));


        std::vector<int> neighbors;
        if (layerObj.contains("neighbours") && layerObj["neighbours"].isArray()) {
            QJsonArray neighborsArray = layerObj["neighbours"].toArray();
            for (const QJsonValue &neighborValue : neighborsArray) {
                neighbors.push_back(neighborValue.toInt());
            }
        }
        allNeighbors.emplace_back(i,neighbors);
    }

    this->layers = layers;

    for (int i = 0; i < layersArray.size(); ++i) {
        layers[i]->setZValue(0);
        addLayer(layers[i], labels[i], defaultTerrain, armies[i],
                 (armies[i].armyType() == ArmyType::HAJDUK)  ? player1 : player2);
        layers[i]->setPos(baseLayer->pos().x() + positions[i].first,
                          baseLayer->pos().y() + positions[i].second);

        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });

        layers[i]->setTroopCount(layerToVertex[layers[i]]->army.getSoldiers());
    }

    if (layers.size() >= 12) {

        for (const auto &layer : allNeighbors){
            int layer_id = layer.first;
            const std::vector<int> &neighbors = layer.second;
            for (int neighbor : neighbors){
                g.insert_edge(layerToVertex[layers[layer_id]], layerToVertex[layers[neighbor]], 1.0);
            }
        }

    }
}

void GameManager::updateLayersId(int PlayerId) {
    for (auto &layer : layers) {
        if (layer) {
            layer->setCurrentPlayer(PlayerId);
        }
    }
}

void GameManager::updateLayersGraphics() {
    clearArrows();
    for (auto &layer : layers) {
        auto vertex = layerToVertex[layer];
        Army army = vertex->army;
        layer->setArmyColor(army.armyType());
        vertex->player.setPlayerId((army.armyType()==ArmyType::HAJDUK) ? 1 : 2);
        layer->setTroopCount(vertex->army.getSoldiers());
    }
}

void GameManager::clearArrows() {
    for (CustomArrowItem* arrow : arrows) {
        scene->removeItem(arrow);
        delete arrow;
    }
    arrows.clear();
}

void GameManager::drawArrow(MapLayer* from, MapLayer* to, int number, int actionId) {
    QPointF fromPos = from->pos() + QPointF((from->boundingRect().width() / 2)-5,
                                            from->boundingRect().height() / 2);
    QPointF toPos = to->pos() + QPointF((to->boundingRect().width() / 2)+20,
                                        to->boundingRect().height() / 2);

    QLineF line(fromPos, toPos);
    CustomArrowItem* arrow = new CustomArrowItem(line,actionId);
    scene->addItem(arrow);
    arrow->setNumber(number);
    arrows.push_back(arrow);
}


void GameManager::addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player) {
    layer->setArmyColor(army.armyType());
    scene->addItem(layer);
    auto vertex = g.insert_vertex(layer->troopText->pos(), label, layer, terrain, army, player);
    layerToVertex[layer] = vertex;
}

void GameManager::transferTroops(MapLayer* from, MapLayer* to, int troops) {
    if (troops > 0 && troops <= from->getTroopCount()) {
        from->setTroopCount(from->getTroopCount() - troops);
        to->setTroopCount(to->getTroopCount() + troops);
    }
}

//print_connections(g, cvor);
void GameManager::printConnections(graph::Vertex* vertex) {
    auto neighbors = g.neighbors(vertex);
    std::cout << "Vertex " << vertex->id() << " is connected to: ";
    for (auto &neighbor : neighbors) {
        std::cout << neighbor->id() << " ";
    }
    std::cout << std::endl;
}

void GameManager::removeArrowByActionId(int actionId) {
    for (size_t i = 0; i < arrows.size(); ++i) {
        if (arrows[i]->getActionId() == actionId) {
            scene->removeItem(arrows[i]);
            delete arrows[i];
            arrows.erase(arrows.begin() + i);
            return;
        }
    }
}


