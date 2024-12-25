#include "clientgamemanager.h"

ClientGameManager::ClientGameManager(QGraphicsScene* scene,QObject* parent)
    : QObject(parent),scene(scene),clientGraph(new graph::Graph()), armyManager(*new AddArmyManager(clientGraph.get()))
{}

void ClientGameManager::initializeUI(QLabel* headerLabel, QPushButton* endTurnButton, QPushButton* moveButton, QPushButton* infoButton,
                  QListWidget* moveList,QPushButton* armyButton,QPushButton* reliefButton,QPushButton* regionsButton,
                  QPushButton*cityButton,QPushButton*cultureButton,QPushButton*defaultButton,NodeInfoWidget* nodeInfoWidget ) {
    this->headerLabel = headerLabel;
    this->endTurnButton = endTurnButton;
    this->moveButton = moveButton;
    this->infoButton = infoButton;
    this->moveList = moveList;
    this->armyButton = armyButton;
    this->defaultButton = defaultButton;
    this->regionsButton = regionsButton;
    this->cityButton = cityButton;
    this->cultureButton = cultureButton;
    this->reliefButton = reliefButton;
    this->nodeInfoWidget = nodeInfoWidget;
    moveList->addItem(QString("Player %1 on turn:").arg(ClientId));
}

void ClientGameManager::initializeGraphics(QJsonObject graphData) {
    QJsonObject rootObj = graphData;

    MapLayer *background = new MapLayer("background",":/resources/Project/Pozadina.png", false);
    MapLayer *baseLayer = new MapLayer("baseLayer",":/resources/Project/Slika.png", false);
    MapLayer *borders = new MapLayer("borders",":/resources/Project/Linije_Granice.png", false);
    MapLayer *water = new MapLayer("water",":/resources/Project/MORE.png", false);
    MapLayer *rivers = new MapLayer("rivers",":/resources/Project/Reke_.png", false);
    MapLayer *relief = new MapLayer("relief",":/resources/Project/reljef.png", false);


    MapLayer *s1 = new MapLayer("s1",":/resources/Project/S1.png", false);
    s1->setColor(QColor(237,188,41,255));
    MapLayer *s2 = new MapLayer("s2",":/resources/Project/S2.png", false);
    s2->setColor(QColor(237,188,41,255));
    MapLayer *s3 = new MapLayer("s3",":/resources/Project/S3.png", false);
    s3->setColor(QColor(159,124,165,255));
    MapLayer *s4 = new MapLayer("s4",":/resources/Project/S4.png", false);
    s4->setColor(QColor(126, 149, 104, 255));
    MapLayer *s5 = new MapLayer("s5",":/resources/Project/S5.png", false);
    s5->setColor(QColor(126, 149, 104, 255));

    s2->setPos(113,0);
    s3->setPos(0,1088);
    s4->setPos(958,1802);
    s5->setPos(2486,1286);
    s1->setOpacity(0.95);
    s2->setOpacity(0.95);
    s3->setOpacity(0.95);
    s4->setOpacity(0.95);
    s5->setOpacity(0.95);

    water->setZValue(1);
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

    QJsonArray layersArray = rootObj["vertices"].toArray();

    std::vector<MapLayer*> layers(layersArray.size());

    for (int i = 0; i < layersArray.size(); ++i) {
        QJsonObject layerObj = layersArray[i].toObject();
        QString label = layerObj.value("label").toString();
        QString labelPath = layerObj.value("label_path").toString();
        QJsonObject positionObj = layerObj.value("position").toObject();
        int posX = positionObj.value("x").toInt();
        int posY = positionObj.value("y").toInt();

        MapLayer* layer = new MapLayer(label,labelPath, true);
        layer->setCurrentPlayer(ClientId);
        layer->setZValue(0);
        layer->setOpacity(0.95);
        layer->setPos(posX, posY);
        scene->addItem(layer);

        layers[i] = layer;
        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });
    }

    this->layers = layers;

    QJsonArray regionsArray = rootObj["regions"].toArray();

    std::map<int,std::vector<std::string>>regionLayers;

    for (const QJsonValue& regionValue : regionsArray) {
        QJsonObject regionObj = regionValue.toObject();
        std::string regionId = regionObj["regionId"].toString().toStdString();
        std::string regionName = regionObj["regionName"].toString().toStdString();

        Region* region = new Region(regionId, regionName);

        QJsonArray regionLayers = regionObj["layers"].toArray();
        for (const QJsonValue& regionLayerValue : regionLayers) {
            std::string layerLabel = regionLayerValue.toString().toStdString();
            for (MapLayer* layer : layers) {
                if (layer->labelName.toStdString() == layerLabel) {
                    region->addLayer(layer, nullptr);
                }
            }
        }

        regions.push_back(region);
    }

    scene->addItem(rivers);
    map = new Map(scene, layerToVertex);
    if (ClientId == 1 && loadGamePath != nullptr) {
        loadGame();
    }
}

void ClientGameManager::applyMapMode(MapMode mode) {
    switch (mode) {
    case MapMode::Relief:
        map->setMainMode(false);
        map->generateReliefMap();
        break;
    case MapMode::Regions:
        map->setMainMode(false);
        map->generateRegionMap();
        break;
    case MapMode::Culture:
        map->setMainMode(false);
        map->generateCultureMap();
        break;
    case MapMode::CityLevel:
        map->setMainMode(false);
        map->generateCityLevelMap();
        break;
    case MapMode::Default:
        map->setMainMode(true);
        map->resetMainGameMap();
        updateFog();
        break;
    }
}

void ClientGameManager::setScene(MapScene *newScene) {
    scene = newScene;
}

void ClientGameManager::setId(int id) {
    ClientId = id;
    player = Player();
    player.setPlayerId(ClientId);
    ClientId == 1 ? player.setArmyType(p1_army) : player.setArmyType(p2_army);
}

void ClientGameManager::processDataFromServer(const QJsonObject& data) {
    if (data.contains("graph") && data["graph"].isObject()) {
        QJsonObject graphData = data["graph"].toObject();
        clientGraph->deserialize(graphData);
        if(!init)
            initializeGraphics(graphData);
    }
    if (data.contains("results") && data["results"].isObject()) {
        QJsonObject resultsObject = data["results"].toObject();
        if (resultsObject.contains("results") && resultsObject["results"].isArray()) {
            QJsonArray resultsArray = resultsObject["results"].toArray();
            for (const QJsonValue& value : resultsArray) {
                if (value.isObject()) {
                    QJsonObject resultJson = value.toObject();
                    Results result;
                    result.fromJson(resultJson);
                    result.printResults();
                    resultsVector.push_back(result);
                    printExplosion(clientGraph->get_vertex_by_id(result.getTargetVertexId()));
                }
            }
        } else {
            qDebug() << "No valid 'results' array found inside the 'results' object.";
        }
    } else {
        qDebug() << "No valid 'results' object found in the data.";
    }
    if(init){
        allReset();
        TurnId = TurnId + 1;
        gameYear.advanceThreeMonths();
        enableInteractions();
    } else if(!init){
        for (auto &layer : layers) {
            graph::Vertex* vertex = clientGraph->get_vertex_by_label(layer->labelName);
            if (vertex) {
                layerToVertex[layer] = vertex;
                vertex->map_layer = layer;

                for (auto& region : regions) {
                    for (auto& regionTerritory : region->getTerritories()) {
                        if (regionTerritory.first->labelName.toStdString() == vertex->label()) {
                            region->setCityForLayer(regionTerritory.first, vertex->city);
                            vertex->region = region;
                            break;
                        }
                    }
                }
            } else {
                qWarning() << "Vertex not found for layer ID:" << layer->getId();
            }
        }
        nodeInfoWidget->updateLayerToVertex(layerToVertex);
        map->updateLayerToVertex(layerToVertex);

        init = true;
    }
    updateGraphics();
    updateFog();
    armyManager.updateRegionOwnership(ClientId, regions);
    armyManager.addTerritory(player);
    armyManager.calculateTotalTroops();

}

QVector<QStringList> ClientGameManager::generateBattleResults() {
    QVector<QStringList> results;

    for (const auto& battle : resultsVector){
        QStringList row;
        row.append(QString::fromStdString(clientGraph->get_vertex_by_id(battle.getTargetVertexId())->label()));
        row.append(QString::number(battle.getDefenderNumber()));
        row.append(QString::number(battle.getAttackerNumber()));
        row.append(QString("%1").arg(battle.getDefenderType() == ArmyType::HAJDUK ? "Hajduk " : "Janissary "));
        row.append(QString("%1").arg(battle.getAttackerType() == ArmyType::HAJDUK ? "Hajduk " : "Janissary "));
        row.append(QString("%1").arg(battle.getWinner()->armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary"));

        results.append(row);
    }
    return results;
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
    actionBuffer.clear();
    QVector<QStringList> resultsData = generateBattleResults();

    dialog = new BattleResultsDialog(resultsVector, nullptr);

    dialog->setResults(resultsData);

    connect(dialog, &BattleResultsDialog::accepted, this, &ClientGameManager::clearExplosions);
    connect(dialog, &BattleResultsDialog::rejected, this, &ClientGameManager::clearExplosions);

    dialog->exec();

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
    clearTemporaryArrows();
    for (auto &layer : layers) {
        if (layerToVertex.find(layer) != layerToVertex.end()) {
            graph::Vertex *vertex = layerToVertex[layer];
            if (vertex) {
                Army army = vertex->army; // Safely access the `army`
                layer->setArmyColor(army.armyType());
                layer->setTroopCount(army.getSoldiers());
                layer->setCurrentPlayer(vertex->player.getPlayerId());
            } else {
                qDebug() << "Vertex is null for layer:" << layer->getId();
            }
        } else {
            qDebug() << "Layer not found in layerToVertex map:" << layer->getId();
        }
    }
    emit gameYearUpdated(gameYear.getCurrentDateString());
}

void ClientGameManager::drawArrow(int playerId, MapLayer* from, MapLayer* to, int number, int actionId, QColor color , bool isTemporary) {
    QPointF fromPos = from->pos() + QPointF((from->boundingRect().width() / 2),
                                            from->boundingRect().height() / 2);
    QPointF toPos = to->pos() + QPointF((to->boundingRect().width() / 2),
                                        to->boundingRect().height() / 2);

    CustomArrowItem* arrow = new CustomArrowItem(fromPos, toPos, actionId, color);

    arrow->setPen(QPen(color, 2));

    arrow->setZValue(1000);

    scene->addItem(arrow);
    if (isTemporary) {
        temporaryArrows.emplace_back(arrow);
    } else {
        arrows[playerId].emplace_back(arrow);
    }

    QRectF arrowBoundingBox = arrow->boundingRect().translated(arrow->pos());
    scene->setSceneRect(scene->sceneRect().united(arrowBoundingBox));
}

std::vector<std::tuple<graph::Vertex*, graph::Edge*, QColor>> ClientGameManager::getValidatedEdges(graph::Vertex* vertex) {
    std::vector<std::tuple<graph::Vertex*, graph::Edge*, QColor>> validatedEdges;

    for (auto& edge : clientGraph->getEdges(vertex)) {
        graph::Vertex* neighbor = (edge->from() == vertex->id())
        ? clientGraph->get_vertex_by_id(edge->to())
        : clientGraph->get_vertex_by_id(edge->from());

        QColor edgeColor;
        if (edge->type() == graph::EdgeType::Sea) {
            edgeColor = Qt::blue;
        } else if (neighbor->player.getPlayerId() == vertex->player.getPlayerId()) {
            edgeColor = Qt::green;
        } else {
            edgeColor = Qt::red;
        }

        validatedEdges.emplace_back(neighbor, edge, edgeColor);
    }

    return validatedEdges;
}

void ClientGameManager::clearTemporaryArrows() {
    for (CustomArrowItem* arrow : temporaryArrows) {
        scene->removeItem(arrow);
        delete arrow;
    }
    temporaryArrows.clear();
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
        std::cout << *it<<std::flush;
        cvor->map_layer->setTroopCount(cvor->army.getSoldiers());
        buffer.erase(it);
        std::cout << "Action with ID " << actionId << " removed for Player " << ClientId << ".\n";
    } else {
        std::cerr << "Action with ID " << actionId << " not found for Player " << ClientId << ".\n";
    }
}
void ClientGameManager::removePlaceAction(int actionId) {
    auto& buffer = actionBuffer;

    auto it = std::find_if(buffer.begin(), buffer.end(),
                           [actionId](const Action& action) {
                               return action.id == actionId;
                           });

    if (it != buffer.end()) {
        graph::Vertex* cvor = clientGraph->get_vertex_by_id(it->sourceVertexId);
        cvor->army.setSoldiers(cvor->army.getSoldiers() - it->soldiers);
        std::cout << *it<<std::flush;
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
    resultsVector.clear();
    for (auto& vertex : clientGraph->vertices ) {
        vertex.second->newRecruits = 0;
    }
    emit endTurnActionsReady(actions, id);
}

void ClientGameManager::clearArrows() {
    for (auto& [playerId, arrowList] : arrows) {
        for (CustomArrowItem* arrow : arrowList) {
            scene->removeItem(arrow);
        }
    }
}

void ClientGameManager::printExplosion(graph::Vertex *target)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int minx = target->map_layer->boundingRect().width() / 4;
    int miny = target->map_layer->boundingRect().height() / 4;

    int expN = 3 +(std::rand() % 8);
    for(int i = 0; i < expN; i++){
        int x = std::rand() % static_cast<int>(target->map_layer->boundingRect().width() / 2);
        int y = std::rand() % static_cast<int>(target->map_layer->boundingRect().height() / 2);
        MapLayer *explosionLayer = new MapLayer("explosionLayer",QString(":/resources/Images/Explosion.png"),false);
        explosions.push_back(explosionLayer);
        explosionLayer->setPos(target->map_layer->pos()+ QPointF(minx+x,miny+y));
        explosionLayer->setZValue(2);
        explosionLayer->setScale(0.04);
        scene->addItem(explosionLayer);
    }

}

Player ClientGameManager::getPlayer() const
{
    return player;
}

graph::Graph*ClientGameManager::getClientGraph() const
{
    return clientGraph.get();
}
void ClientGameManager::clearExplosions() {
    for (auto& explosion : explosions) {
        scene->removeItem(explosion);
    }
}

void ClientGameManager::updateFog()
{
    for (auto &layer : layers) {
        auto ver = layerToVertex[layer];
        bool playerIsClient = ver->player.getPlayerId() == ClientId;

        if (!playerIsClient) {
            layer->setTroopTextVisible(false);
            QColor fogColor = layer->getFogColor();
            layer->setFogOfWar(fogColor);

            bool hasClientNeighbor = false;
            for (auto &vertex : clientGraph->neighbors(ver)) {
                if (vertex->player.getPlayerId() == ClientId) {
                    hasClientNeighbor = true;
                    break;
                }
            }

            if (hasClientNeighbor) {
                layer->setTroopTextVisible(true);
                layer->setColor(layer->getArmyColor());
            }
        } else {
            layer->setTroopTextVisible(true);
        }
    }
}


Year ClientGameManager::year(){
    return gameYear;
}

void ClientGameManager::saveGame() {
    QString directoryPath = "../../resources/saved_games/";

    QDir dir(directoryPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    int nextFileNumber = 1;
    QRegularExpression regex("^game(\\d+)\\.json$");
    for (const QFileInfo& fileInfo : dir.entryInfoList({"game*.json"}, QDir::Files)) {
        QRegularExpressionMatch match = regex.match(fileInfo.fileName());
        if (match.hasMatch()) {
            int fileNumber = match.captured(1).toInt();
            if (fileNumber >= nextFileNumber) {
                nextFileNumber = fileNumber + 1;
            }
        }
    }

    QString defaultFileName = QString("game%1.json").arg(nextFileNumber);

    QString filePath = QFileDialog::getSaveFileName(
        nullptr,
        "Save Game",
        directoryPath + defaultFileName,
        "JSON Files (*.json);;All Files (*)"
        );

    if (filePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Save Cancelled", "No file selected. Save operation cancelled.");
        return;
    }

    clientGraph->save_to_json(filePath.toStdString());
}

void ClientGameManager::loadGame() {
    QString fullPath = "../../resources/saved_games/" + loadGamePath;

    // QString filePath = QFileDialog::getOpenFileName(
    //     nullptr,
    //     "Load Saved Game",
    //     directoryPath,
    //     "JSON Files (*.json);;All Files (*)"
    //     );

    // if (filePath.isEmpty()) {
    //     QMessageBox::warning(nullptr, "Load Cancelled", "No file selected. Load operation cancelled.");
    //     return;
    // }

    // if (!fileManager.fileExists(filePath)) {
    //     qWarning() << "Saved game file does not exist:" << filePath;
    //     QMessageBox::critical(nullptr, "Load Failed", "Selected file does not exist:\n" + filePath);
    //     return;
    // }

    QJsonObject graphData = fileManager.loadFromFile(fullPath);
    if (graphData.isEmpty()) {
        qWarning() << "Failed to load game state. File might be corrupt.";
        QMessageBox::critical(nullptr, "Load Failed", "Failed to load game state. File might be corrupt.");
        return;
    }

    emit gameDataLoaded(graphData);

    // clientGraph->deserialize(graphData);

    // for (auto &layer : layers) {
    //     graph::Vertex *vertex = clientGraph->get_vertex_by_id(layer->getId() + 1);
    //     if (vertex) {
    //         layerToVertex[layer] = vertex;
    //         vertex->map_layer = layer;
    //     }
    // }

    // updateGraphics();
}

void ClientGameManager::processLoadData(const QJsonObject &gameData) {
    qDebug() << "Processing loaded game data:" << gameData;

    clientGraph->deserialize(gameData);

    // for (auto &layer : layers) {
    //     graph::Vertex *vertex = clientGraph->get_vertex_by_id(layer->getId() + 1);
    //     if (vertex) {
    //         layerToVertex[layer] = vertex;
    //         vertex->map_layer = layer;
    //     }
    // }

    updateGraphics();
    qDebug() << "Game data successfully processed and applied.";
}

int ClientGameManager::getClientId() const {
    return ClientId;
}


