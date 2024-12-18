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

void ClientGameManager::processDataFromServer(const QJsonObject& data) {
    if (data.contains("graph") && data["graph"].isObject()) {
        QJsonObject graphData = data["graph"].toObject();
        clientGraph->deserialize(graphData);
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
            qWarning() << "No valid 'results' array found inside the 'results' object.";
        }
    } else {
        qWarning() << "No valid 'results' object found in the data.";
    }

    if(init){
        allReset();
        TurnId = TurnId + 1;
        enableInteractions();
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
    //qDebug() << "Current Game Date:" << gameYear.getCurrentDateString();
    gameYear.advanceThreeMonths();
    updateGraphics();
}

QVector<QStringList> ClientGameManager::generateBattleResults() {
    QVector<QStringList> results;

    for (const auto& battle : resultsVector){
        QStringList row;
        row.append(QString::number(battle.getTargetVertexId()));
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
    emit gameYearUpdated(gameYear.getCurrentDateString());
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
    resultsVector.clear();
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
        MapLayer *explosionLayer = new MapLayer(150,QString(":/resources/Images/Explosion.png"),false);
        explosions.push_back(explosionLayer);
        explosionLayer->setPos(target->map_layer->pos()+ QPointF(minx+x,miny+y));
        explosionLayer->setZValue(2);
        explosionLayer->setScale(0.04);
        scene->addItem(explosionLayer);
    }

}
void ClientGameManager::clearExplosions() {
    for (auto& explosion : explosions) {
        scene->removeItem(explosion);
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
    QString directoryPath = "../../resources/saved_games/";

    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "Load Saved Game",
        directoryPath,
        "JSON Files (*.json);;All Files (*)"
        );

    if (filePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Load Cancelled", "No file selected. Load operation cancelled.");
        return;
    }

    if (!fileManager.fileExists(filePath)) {
        qWarning() << "Saved game file does not exist:" << filePath;
        QMessageBox::critical(nullptr, "Load Failed", "Selected file does not exist:\n" + filePath);
        return;
    }

    QJsonObject graphData = fileManager.loadFromFile(filePath);
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
