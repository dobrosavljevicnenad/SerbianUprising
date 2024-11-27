#include "gamemanager.h"

GameManager::GameManager(QGraphicsScene* scene) : scene(scene), ma(g)  {}

void GameManager::initializeMap(){
    /*
    auto neigh = g.neighbors(v1);
    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }

    v2->army.setSoldiers(10);

    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }
    */
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
    std::vector<Player> players;
    std::vector<std::string> labels;
    std::vector<std::pair<int,int>>positions;
    std::vector<std::pair<int,std::vector<int>>> allNeighbors;



    MapLayer *baseLayer = new MapLayer(":/resources/base.png", false);
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

        players.emplace_back((type==ArmyType::HAJDUK) ? 1 : 2, type);


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
    for (int i = 0; i < layersArray.size(); ++i) {
        layers[i]->setZValue(0);
        addLayer(layers[i], labels[i], defaultTerrain, armies[i], players[i]);
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
                //std::cout<<layer_id<<"-->"<<neighbor<<std::endl;
                g.insert_edge(layerToVertex[layers[layer_id]], layerToVertex[layers[neighbor]], 1.0);
            }
        }
    }
}//12 i 13 (11 i 12) 6 i 5 (5 i 4)


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
