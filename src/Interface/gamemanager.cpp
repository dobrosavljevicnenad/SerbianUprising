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

    QJsonArray layersArray = rootObj["vertices"].toArray();

    std::vector<MapLayer*> layers(layersArray.size());
    std::vector<Army> armies;

    Player player1(1,ArmyType::HAJDUK);
    Player player2(2,ArmyType::JANISSARY);

    std::vector<std::string> labels;
    std::vector<std::pair<int,int>>positions;
    std::vector<std::pair<int,std::string>>verticesId;

    MapLayer *background = new MapLayer(":/resources/Project/Pozadina.png", false);
    MapLayer *baseLayer = new MapLayer(":/resources/Project/Slika.png", false);
    MapLayer *borders = new MapLayer(":/resources/Project/Linije_Granice.png", false);
    MapLayer *water = new MapLayer(":/resources/Project/MORE.png", false);
    MapLayer *rivers = new MapLayer(":/resources/Project/Reke_.png", false);
    MapLayer *relief = new MapLayer(":/resources/Project/reljef.png", false);


    MapLayer *s1 = new MapLayer(":/resources/Project/S1.png", false);
    MapLayer *s2 = new MapLayer(":/resources/Project/S2.png", false);
    MapLayer *s3 = new MapLayer(":/resources/Project/S3.png", false);
    MapLayer *s4 = new MapLayer(":/resources/Project/S4.png", false);
    MapLayer *s5 = new MapLayer(":/resources/Project/S5.png", false);

    s2->setPos(113,0);
    s3->setPos(0,1088);
    s4->setPos(958,1802);
    s5->setPos(2486,1286);
    s1->setOpacity(0.95);
    s2->setOpacity(0.95);
    s3->setOpacity(0.95);
    s4->setOpacity(0.95);
    s5->setOpacity(0.95);


    background->setZValue(-1);
    baseLayer->setZValue(-1);

    scene->addItem(background);
    scene->addItem(baseLayer);
    scene->addItem(relief);
    scene->addItem(borders);
    scene->addItem(water);

    scene->addItem(s1);
    scene->addItem(s2);
    scene->addItem(s3);
    scene->addItem(s4);
    scene->addItem(s5);

    std::vector<Terrain> terrains;
    Terrain defaultTerrain(TerrainType::MOUNTAIN);

    for (int i = 0; i <layersArray.size(); ++i) {
        QJsonObject layerObj = layersArray[i].toObject();

        std::string label = layerObj.value("label").toString().toStdString();
        labels.push_back(label);

        QString labelPath = layerObj.value("label_path").toString();
        std::string armyType = layerObj.value("army_type").toString().toStdString();

        int numOfSoldiers = layerObj.value("num_of_soldiers").toInt();
        int id = layerObj.value("id").toInt();

        int cityLevel = layerObj.value("cityLevel").toInt();

        verticesId.push_back(make_pair(i,label));
        layers[i] = (new MapLayer(labelPath, true));

        std::string terrain = layerObj.value("terrain_type").toString().toStdString();

        TerrainType terrainType;
        if (terrain == "HILL") terrainType = TerrainType::HILL;
        else if (terrain == "FIELD") terrainType = TerrainType::FIELD;
        else if (terrain == "FOREST") terrainType = TerrainType::FOREST;
        else terrainType = TerrainType::MOUNTAIN;

        Terrain terrainObj(terrainType);
        terrains.emplace_back(terrainObj);

        ArmyType type = (armyType == "HAJDUK") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        armies.emplace_back(numOfSoldiers,type);
        QJsonObject positionObj = layerObj.value("position").toObject();

        int posX = positionObj.value("x").toInt();
        int posY = positionObj.value("y").toInt();
        positions.push_back(std::make_pair(posX,posY));

    }

    this->layers = layers;
    //std::cout<<layersArray.size()<<std::endl;

    for (int i = 0; i < layersArray.size(); ++i) {
        layers[i]->setZValue(0);
        addLayer(layers[i], labels[i], terrains[i], armies[i],
                 (armies[i].armyType() == ArmyType::HAJDUK)  ? player1 : player2);
        layers[i]->setPos(baseLayer->pos().x() + positions[i].first,
                          baseLayer->pos().y() + positions[i].second);
        layers[i]->setOpacity(0.95);

        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });

        layers[i]->setTroopCount(layerToVertex[layers[i]]->army.getSoldiers());
    }


    QJsonArray edgesArray = rootObj["edges"].toArray();

    for(int i = 0 ; i < edgesArray.size();i++){
        QJsonObject edge = edgesArray[i].toObject();

        int from = edge["from"].toInt();
        int to = edge["to"].toInt();
        double weight = edge["weight"].toDouble();

        std::string type = edge["type"].toString().toStdString();

        EdgeType edgeType;
        if(type == "River"){
             edgeType = EdgeType::River;
        }else{
             edgeType = EdgeType::Land;
        }


        Vertex* fromVertex = layerToVertex[layers[from-1]];//layer's index starts from 0, but id starts from 1
        Vertex* toVertex = layerToVertex[layers[to-1]];//layer's index starts from 0, but id starts from 1


        g.insert_edge(fromVertex, toVertex,weight, edgeType);
    }

    //inicijalizacija regiona

    QJsonArray regionsArray = rootObj["regions"].toArray();


    std::vector<Region>regions;
    std::vector<std::pair<int,std::vector<std::string>>>niz_lejera;
    std::vector<std::string>layer_string;
    std::map<int,std::vector<std::string>>regionLayers;

    for(int i = 0;i<regionsArray.size();i++){
        QJsonObject regionObj = regionsArray[i].toObject();
        std::string regionId = regionObj["regionId"].toString().toStdString();
        std::string regionName = regionObj["regionName"].toString().toStdString();

        regions.emplace_back(Region(regionId,regionName));

        QJsonArray layersArray = regionObj["layers"].toArray();

        for(const QJsonValue &layerValue : layersArray){
            MapLayer *mp ;
            for(const auto &x : verticesId){
                if(layerValue.toString().toStdString() == x.second){
                    mp = layers[x.first];
                }
            }
            regions[i].territories.push_back(mp);
            layer_string.push_back(layerValue.toString().toStdString());
        }

        niz_lejera.push_back(std::make_pair(i,layer_string));
        layer_string.clear();
    }
    /*for(int i = 0;i<regions.size();i++)   {
        std::cout << "Region ID: " << regions[i].getRegionId() << "\n";
        std::cout << "Region Name: " << regions[i].getRegionName() << "\n";
        std::cout<<"Sastoji se od sledecih lejera"  << "\n";
        for(std::string &x : niz_lejera[i].second){
            std::cout<<x<< " ";
        }
        std::cout<<std::endl;
        std::cout << "--------------------------\n";
    }*/
    scene->addItem(rivers);


    armyManagers[1] = AddArmyManager();
    armyManagers[2] = AddArmyManager();
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
    for (auto& [playerId, arrowList] : arrows) {
        for (CustomArrowItem* arrow : arrowList) {
            scene->removeItem(arrow);
        }
    }
}

void GameManager::drawArrow(int playerId, MapLayer* from, MapLayer* to, int number, int actionId) {
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

void GameManager::filterAndRedrawArrows(int currentPlayerId) {
    clearArrows();

    auto it = arrows.find(currentPlayerId);
    if (it != arrows.end()) {
        for (CustomArrowItem* arrow : it->second) {
            scene->addItem(arrow);
        }
    }
}

AddArmyManager& GameManager::getArmyManager(int playerId) {
    auto it = armyManagers.find(playerId);
    if (it != armyManagers.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid player ID for ArmyManager");
    }
}


