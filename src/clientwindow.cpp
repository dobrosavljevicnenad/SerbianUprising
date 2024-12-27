#include "clientwindow.h"
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include<QListWidget>
#include<QDebug>
#include <qapplication.h>
#include <qgraphicseffect.h>
#include <QKeyEvent>
#include "Interface/Items/custominputdialog.h"
#include "Interface/Items/attacktransferdialog.h"

ClientWindow::ClientWindow(ClientGameManager *existingGameManager,QWidget *parent)
    : QMainWindow(parent),
    scene(new MapScene(this)),
    gameManager(existingGameManager),
    selectedLayer(nullptr),
    moveList(new QListWidget())
{
    this->showMaximized();

    setupGame();
    setupUI();
    connectSignals();
    gameManager->initializeUI(headerLabel, endTurnButton, moveButton, infoButton,moveList,
                              armyButton,reliefButton,regionsButton,cityButton,cultureButton,
                              defaultButton,nodeInfoWidget,characterWidget);
    connect(gameManager, &ClientGameManager::gameYearUpdated, this, &ClientWindow::updateYearLabel);

    // Dodajte u konstruktor ClientWindow-a nakon connectSignals()
    QTimer* serverCheckTimer = new QTimer(this);
    connect(serverCheckTimer, &QTimer::timeout, this, &ClientWindow::checkServerClosed);
    serverCheckTimer->start(100); // Proverava svakih 100ms
}

ClientWindow::~ClientWindow() {
    qDebug() << "Destroying ClientWindow...";
}

void ClientWindow::checkServerClosed() {
    if (gameManager->server_closed) {
        freezeUI();
        showDisconnectPauseMenu();
        gameManager->server_closed=false;
    }
}

// Funkcija za zamrzavanje UI-a
void ClientWindow::freezeUI() {
    QList<QWidget*> layoutWidgets = {
        headerLabel, infoButton, moveButton, armyButton, endTurnButton,
        moveList, yearDisplayLabel, reliefButton, regionsButton,
        cityButton, cultureButton, defaultButton
    };

    for (auto* widget : layoutWidgets) {
        widget->setEnabled(false); // Onemogućava interakciju
    }
}


void ClientWindow::setupGame() {
    view = new ZoomableGraphicsView(this);
    view->setScene(scene);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    view->updateGeometry();
    setCentralWidget(view);
    gameManager->setScene(scene);

}

void ClientWindow::setupUI() {

    nodeInfoWidget = new NodeInfoWidget(gameManager->layerToVertex,this);
    nodeInfoWidget->hide();

    layoutContainer = new QWidget(view->viewport());
    if(gameManager->ClientId == 1){
        layoutContainer->setStyleSheet("background-color: rgba(74, 47, 47,190);"
                                       "border-radius: 0px;"
                                       "border-image: url(:/resources/border1.png) 30 stretch;");
    }
    else{
        layoutContainer->setStyleSheet("background-color: rgba(3, 66, 5,190);"
                                       "border-radius: 0px;"
                                       "border-image: url(:/resources/border1.png) 30 stretch;");
    }
    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    // Header Label
    QString turnLabel = QString("Turn %1").arg(1);
    mainLayout->setContentsMargins(0, 20, 0, 0);
    headerLabel = new QLabel(turnLabel);
    QFont font = headerLabel->font();
    font.setBold(true);
    font.setPointSize(14);
    headerLabel->setFont(font);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: #FFD700; background-color: transparent; border-image: none;");
    mainLayout->addWidget(headerLabel);

    // Year Label
    QString yearLabel = QString("Year: %1").arg(gameManager->year().getCurrentDateString());
    yearDisplayLabel = new QLabel(yearLabel);
    QFont yearFont = yearDisplayLabel->font();
    yearFont.setBold(true);
    yearFont.setPointSize(12);
    yearDisplayLabel->setFont(yearFont);
    yearDisplayLabel->setAlignment(Qt::AlignCenter);
    yearDisplayLabel->setStyleSheet("color: #D4AF37; background-color: transparent; border-image: none;");
    mainLayout->addWidget(yearDisplayLabel);


    // Buttons Row
    QHBoxLayout *buttonRowLayout = new QHBoxLayout();
    infoButton = new QPushButton("Info");
    moveButton = new QPushButton("Move");
    armyButton = new QPushButton("Army");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: #3A5F0B; "
        "   border-radius: 15px; "
        "   color: white;   "
        "   border: 2px solid #2E4600; "
        "   font-size: 10pt; "
        "   padding: 5px;   "
        "   border-image: none; "
        "} "
        "QPushButton:hover { "
        "   background-color: #4F7942; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2E4600; "
        "} ";

    infoButton->setStyleSheet(buttonStyle);
    moveButton->setStyleSheet(buttonStyle);
    armyButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #2E4600; "
        "   border-image: none; "
        "} "
        );

    infoButton->setFixedSize(44, 40);
    moveButton->setFixedSize(44, 40);
    armyButton->setFixedSize(44, 40);
    this->activeButton = armyButton;

    buttonRowLayout->addWidget(infoButton);
    buttonRowLayout->addWidget(moveButton);
    buttonRowLayout->addWidget(armyButton);
    buttonRowLayout->setSpacing(3);
    buttonRowLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(buttonRowLayout);

    // End Turn Button
    endTurnButton = new QPushButton("End Turn");
    endTurnButton->setFixedSize(160, 30);
    endTurnButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #8B0000; "
        "   border-radius: 10px; "
        "   color: white;   "
        "   border: 2px solid #5A0000; "
        "   font-size: 11pt; "
        "   padding: 5px;   "
        "   border-image: none; "
        "} "
        "QPushButton:hover { "
        "   background-color: #B22222; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #5A0000; "
        "} "
        );
    mainLayout->addWidget(endTurnButton, 0, Qt::AlignCenter);

    // Move List
    moveList = new QListWidget(view->viewport());
    moveList->setFixedSize(200, 300);
    moveList->setStyleSheet(
        "QListWidget { "
        "   background-color: #F5DEB3; "
        "   border-radius: 5px; "
        "   border: 1px solid #A0522D; "
        "   color: #000;    "
        "   font-size: 10pt; "
        "   border-image: none; "
        "} "
        "QListWidget::item:hover { "
        "   background-color: #FFE4C4; "
        "} "
        "QListWidget::item:selected { "
        "   background-color: #D2B48C;"
        "   color: black; "
        "} "
        );
    mainLayout->addWidget(moveList, 0, Qt::AlignCenter);

    QPushButton* toggleButton = new QPushButton("⮝", layoutContainer);
    toggleButton->setFixedSize(40, 40);
    toggleButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #D2B48C; "
        "   border-radius: 20px; "
        "   border: 2px solid #8B4513;"
        "   color: black;"
        "   font-size: 12pt;"
        "   border-image: none; "
        "} "
        "QPushButton:hover { "
        "   background-color: #F5DEB3; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #8B4513; "
        "   color: white; "
        "}"
        );

    mainLayout->addWidget(toggleButton, 0, Qt::AlignCenter);

    layoutContainer->setLayout(mainLayout);
    layoutContainer->setGeometry(10, 10, 250, 520);

    mapModeContainer = new QWidget(view->viewport());
    if(gameManager->ClientId == 1){
        mapModeContainer->setStyleSheet(
            "background-color: rgba(74, 47, 47,190); "
            "border-radius: 10px;"
            "border-image: url(:/resources/border1.png) 10 stretch;"
            "padding: 5px;"
            );
    }
    else{
        mapModeContainer->setStyleSheet(
            "background-color: rgba(3, 66, 5,190); "
            "border-radius: 10px;"
            "border-image: url(:/resources/border1.png) 10 stretch;"
            "padding: 5px;"
            );
    }

    QHBoxLayout* mapModeLayout = new QHBoxLayout();
    mapModeLayout->setSpacing(8);
    mapModeLayout->setContentsMargins(10, 10, 10, 10);

    // Map Mode Buttons
    reliefButton = new QPushButton("Relief");
    regionsButton = new QPushButton("Regions");
    cityButton = new QPushButton("City");
    cultureButton = new QPushButton("Culture");
    defaultButton = new QPushButton("Main");

    QString mapButtonStyle =
        "QPushButton { "
        "   background-color: #2E4600; "
        "   border-image: none; "
        "   color: white; "
        "   border-radius: 10px; "
        "   border: 2px solid #1B3400;"
        "   padding: 5px 10px; "
        "   font-size: 10pt; "
        "} "
        "QPushButton:hover { "
        "   background-color: #4F7942; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #8B0000; "
        "}";

    reliefButton->setStyleSheet(mapButtonStyle);
    regionsButton->setStyleSheet(mapButtonStyle);
    cityButton->setStyleSheet(mapButtonStyle);
    cultureButton->setStyleSheet(mapButtonStyle);
    defaultButton->setStyleSheet(mapButtonStyle);

    reliefButton->setFixedSize(80, 40);
    regionsButton->setFixedSize(80, 40);
    cityButton->setFixedSize(80, 40);
    cultureButton->setFixedSize(80, 40);
    defaultButton->setFixedSize(80, 40);

    mapModeLayout->addWidget(reliefButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(regionsButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(cityButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(cultureButton,0, Qt::AlignCenter);
    mapModeLayout->addWidget(defaultButton,0, Qt::AlignCenter);

    mapModeContainer->setLayout(mapModeLayout);

    characterContainer = new QWidget(view->viewport());
    QVBoxLayout *characterLayout = new QVBoxLayout();
    characterWidget = new CharacterWidget(gameManager->ClientId);
    characterLayout->addWidget(characterWidget);
    characterContainer->setLayout(characterLayout);
    characterContainer->setFixedSize(456, 246);
    characterContainer->setContentsMargins(0, 0, 0, 0);

    initrepositionFixedWidgets();

    QList<QWidget*> layoutWidgets = {headerLabel, infoButton, moveButton, armyButton, endTurnButton, moveList, yearDisplayLabel};

    connect(toggleButton, &QPushButton::clicked, this, [=]() {
        static bool isExpanded = true;

        if (isExpanded) {
            for (auto* widget : layoutWidgets) {
                widget->hide();
            }
            layoutContainer->setFixedSize(250, toggleButton->height() + 20);
            layoutContainer->setStyleSheet("background-color: transparent");
            toggleButton->setText("⮟");
        } else {
            for (auto* widget : layoutWidgets) {
                widget->show();
            }
            layoutContainer->setFixedSize(250, 520);
            layoutContainer->setStyleSheet("background-color: rgba(0, 0, 0, 128);"
                                           "border-radius: 15px;"
                                           "border-image: url(:/resources/border1.png) 30 stretch;");
            toggleButton->setText("⮝");
        }
        isExpanded = !isExpanded;
    });

}


void ClientWindow::updateYearLabel(QString newYear) {
    yearDisplayLabel->setText(newYear);
}
void ClientWindow::connectSignals() {
    connect(reliefButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Relief);
    });
    connect(regionsButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Regions);
    });
    connect(cityButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::CityLevel);
    });
    connect(cultureButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Culture);
    });
    connect(defaultButton, &QPushButton::clicked, this, [this]() {
        gameManager->applyMapMode(ClientGameManager::MapMode::Default);
    });

    connect(view->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });
    connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });
    connect(gameManager, &ClientGameManager::layerClicked, this, &ClientWindow::onLayerClicked);
    connect(endTurnButton, &QPushButton::clicked, this, &ClientWindow::onEndTurnClicked);
    connect(moveList, &QListWidget::itemClicked, this, &ClientWindow::onMoveClicked);
    connect(infoButton, &QPushButton::clicked, this, &ClientWindow::onInfoButtonClicked);
    connect(moveButton, &QPushButton::clicked, this, &ClientWindow::onMoveButtonClicked);
    connect(armyButton, &QPushButton::clicked, this, &ClientWindow::onPlaceButtonClicked);
}

void ClientWindow::onEndTurnClicked() {
    if (!gameManager) {
        qDebug() << "Game manager is not available!";
        return;
    }
    if(gameManager->getArmyManager().totalTroops != 0){
        CustomMessageBox::showMessage("Deploy all army first!", this);
        return;
    }
    QVector<Action> actions = gameManager->actionBuffer;
    int clientId = gameManager->ClientId;

    gameManager->EndTurnClicked(actions, clientId);
    gameManager->disableInteractions();
}

void ClientWindow::initrepositionFixedWidgets()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();

    characterContainer->move(screenRect.width() - characterContainer->width() - 57, -10);
    mapModeContainer->move(10, screenRect.height() - characterContainer->height()+111);
}

void ClientWindow::repositionFixedWidgets()
{

    QRect viewportRect = view->viewport()->rect();
    characterContainer->move(view->viewport()->rect().topRight() - QPoint(characterContainer->width()-10, +10));
    layoutContainer->move(viewportRect.topLeft() + QPoint(10, 10));
    mapModeContainer->move(viewportRect.bottomLeft() + QPoint(10, -(mapModeContainer->height()+10)));
}

void ClientWindow::processEndTurnClicked(){
     // Show dialog modally
}

void ClientWindow::onMoveClicked(QListWidgetItem* item) {
    if (!item) return;

    QString actionType = item->data(Qt::UserRole + 2).toString();

    if (actionType == "Move") {
        QVariant data = item->data(Qt::UserRole);
        if (!data.isValid()) return;

        int actionId = data.toInt();
        gameManager->removeActionById(actionId);
        gameManager->removeArrowByActionId(actionId);
        delete item;
    } else if (actionType == "Place") {
        int layerId = item->data(Qt::UserRole).toInt();
        int troopsToRemove = item->data(Qt::UserRole + 1).toInt();

        MapLayer* layer = nullptr;
        for (auto pair : gameManager->layerToVertex) {
            if (pair->id() == layerId) {
                layer = pair->map_layer;
                break;
            }
        }

        if (layer) {
            AddArmyManager& armyManager = gameManager->getArmyManager();
            armyManager.decreaseAvailableTroops(-troopsToRemove);
            gameManager->layerToVertex[layer]->newRecruits -= troopsToRemove;
            QVariant data = item->data(Qt::UserRole + 3);
            int actionId = data.toInt();
            gameManager->removePlaceAction(actionId);
        }

        delete moveList->takeItem(moveList->row(item));
    }
}

void ClientWindow::onLayerClicked(MapLayer* layer) {

    if (activeButton == moveButton) {
        handleMoveArmy(layer);
    } else if (activeButton == armyButton) {
        handlePlaceArmy(layer);
    } else if (activeButton == infoButton) {
        handleInfomation(layer);
    } else {
        CustomMessageBox::showMessage("This action is not supported.", this);
    }

}

void ClientWindow::handleInfomation(MapLayer* layer){
    if (layer) {
        nodeInfoWidget->updateNodeInfo(layer);
        QPoint bottomRight = view->viewport()->rect().bottomRight();
        QPoint globalPos = view->mapToGlobal(bottomRight) - QPoint(nodeInfoWidget->width() + 10, nodeInfoWidget->height() + 10);
        nodeInfoWidget->move(globalPos);
        nodeInfoWidget->show();
    } else {
        CustomMessageBox::showMessage("Info: No layer selected!", this);
    }
}

void ClientWindow::handleMoveArmy(MapLayer* layer){
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        if(selected_vertex->player.getPlayerId() != gameManager->ClientId){
            selectedLayer = nullptr;
            return ;
        }

        gameManager->clearTemporaryArrows();

        auto validatedEdges = gameManager->getValidatedEdges(selected_vertex);
        for (const auto& [neighbor, edge, color] : validatedEdges) {
            MapLayer* neighborLayer = neighbor->map_layer;
            gameManager->drawArrow(gameManager->ClientId, selectedLayer, neighborLayer, 0, 0, color, true);
        }

    } else {
        if(selectedLayer == layer) {
            CustomMessageBox::showMessage("Error: You selected the same layer. Select another layer.", this);
            selectedLayer = nullptr;
            gameManager->clearTemporaryArrows();
            return ;
        }
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        graph::Vertex* vertex = gameManager->layerToVertex[layer];

        bool isNeighbor = false;
        graph::Edge* connectingEdge = nullptr;
        auto validatedEdges = gameManager->getValidatedEdges(selected_vertex);
        for (const auto& [neighbor, edge, color] : validatedEdges) {
            if (neighbor == vertex) {
                isNeighbor = true;
                connectingEdge = edge;
                break;
            }
        }

        if (!isNeighbor) {
            CustomMessageBox::showMessage("Error: You must select a neighboring node.", this);

            selectedLayer = nullptr;
            gameManager->clearTemporaryArrows();
            return;
        }

        if (connectingEdge->type() == graph::EdgeType::Sea &&
            selected_vertex->player.getPlayerId() != vertex->player.getPlayerId()) {
            CustomMessageBox::showMessage("Error: Cannot move across sea to a different player's territory.", this);
            return;
        }

        int maxTroops = selected_vertex->army.getSoldiers();
        int troopsToTransfer = 0;
        AttackTransferDialog dialog(
            QString::fromStdString(selected_vertex->label()),
            QString::fromStdString(vertex->label()),
            selected_vertex->army.getSoldiers(),
            vertex->army.getSoldiers(),
            selected_vertex->map_layer->get_m_originalPixmap(),
            vertex->map_layer->get_m_originalPixmap()
            );

        if (dialog.exec() == QDialog::Accepted){
             troopsToTransfer = dialog.getSelectedArmies();
            if (troopsToTransfer == 0) {
                CustomMessageBox::showMessage("Error: Transferring 0 troops is not allowed.", this);
                selectedLayer = nullptr;
                gameManager->clearTemporaryArrows();
                return;
            }
        }
        else{
            selectedLayer = nullptr;
            gameManager->clearTemporaryArrows();
            return;
        }

        ActionType type = (selected_vertex->army.armyType() == vertex->army.armyType()) ? ActionType::MOVE_ARMY : ActionType::ATTACK;
        if (selected_vertex == vertex)
            type = ActionType::PLACE_ARMY;

        int pid = gameManager->ClientId;
        int source = selected_vertex->id();
        int target = vertex->id();

        Action newAction(type, pid, source, target, troopsToTransfer);

        selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
        selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());

        QColor actionColor;
        if (type == ActionType::ATTACK) {
            actionColor = Qt::red;
        } else {
            actionColor = Qt::green;
        }
        gameManager->clearTemporaryArrows();
        gameManager->drawArrow(gameManager->ClientId, selectedLayer, layer, troopsToTransfer, newAction.id, actionColor);
        gameManager->addAction(newAction);

        QString move = gameManager->GetCurrentAction(newAction);
        QListWidgetItem* item = new QListWidgetItem(move);
        item->setData(Qt::UserRole, newAction.id);
        item->setData(Qt::UserRole + 2, "Move");
        moveList->addItem(item);

        selectedLayer = nullptr;
    }
}

void ClientWindow::handlePlaceArmy(MapLayer* layer) {
    int currentPlayerId = gameManager->ClientId;
    graph::Vertex* selected_vertex = gameManager->layerToVertex[layer];
    if (selected_vertex->player.getPlayerId() == currentPlayerId) {
        AddArmyManager& armyManager = gameManager->getArmyManager();

        if (armyManager.totalTroops == 0) {
            CustomMessageBox::showMessage("Error: Not enough available soldiers.", this);
            selectedLayer = nullptr;
            return;
        }

        Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
        int troopsToAdd = 0;
        int troops = 0;
        switch (selected_vertex->city->getLevel()) {
        case 3:
            troops = std::numeric_limits<int>::max();
            break;
        case 2:
            troops = 50;
            break;
        default:
            troops = 10;
            break;
        }
        troops = std::min(armyManager.totalTroops, troops - selected_vertex->newRecruits);
        if(troops == 0){
            CustomMessageBox::showMessage("Cant add more soldiers on this territory", this);
            selected_vertex = nullptr;
            return;
        }
        if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier)) {
            troopsToAdd = std::min(100, troops);
        } else if (modifiers == Qt::ShiftModifier) {
            troopsToAdd = std::min(10, troops);
        } else if (modifiers == Qt::ControlModifier) {
            troopsToAdd = std::min(1, troops);
        } else {
            std::string message = "Enter the number of soldiers up to " + std::to_string(troops) + " to place:";
            /*troopsToAdd = QInputDialog::getInt(this, tr("Place Army"),
                                               tr(message.c_str()), 0, 0, troops, 1, &ok);
            if (!ok || troopsToAdd <= 0) {
                selectedLayer = nullptr;
                return;
            }*/
            CustomInputDialog dialog("Place Army", QString::fromStdString(message), nullptr, troops);
            if(dialog.exec() == QDialog::Accepted){
                troopsToAdd = dialog.getInputInt();
                if(troopsToAdd <= 0){
                    selected_vertex = nullptr;
                    return;
                }
            }
            else{
                selected_vertex = nullptr;
                return;
            }
        }

        if (troopsToAdd > 0) {
            layer->setTroopCount(layer->getTroopCount() + troopsToAdd);
            int pid = gameManager->ClientId;
            int source = selected_vertex->id();
            bool found = false;
            for(auto& action : gameManager->actionBuffer){
                if(action.sourceVertexId == source && action.type == ActionType::PLACE_ARMY){
                    QString moveDescription = QString("Placed %1 troops on %2").arg(action.soldiers).arg(
                        QString::fromStdString(selected_vertex->label()));
                    action.soldiers += troopsToAdd;
                    auto items = moveList->findItems(moveDescription, Qt::MatchExactly);
                    if (!items.isEmpty()) {
                        QListWidgetItem* itemToRemove = items.first();
                        moveList->removeItemWidget(itemToRemove);
                        delete itemToRemove;
                    }
                    moveDescription = QString("Placed %1 troops on %2").arg(action.soldiers).arg(
                        QString::fromStdString(selected_vertex->label()));
                    QListWidgetItem* newItem = new QListWidgetItem(moveDescription);
                    newItem->setData(Qt::UserRole, QVariant::fromValue(selected_vertex->id()));
                    newItem->setData(Qt::UserRole + 1, QVariant(action.soldiers));
                    newItem->setData(Qt::UserRole + 2, "Place");
                    newItem->setData(Qt::UserRole + 3, action.id);
                    moveList->addItem(newItem);
                    found = true;
                }
            }
            if(!found){
                Action newAction(ActionType::PLACE_ARMY, pid, source, 0, troopsToAdd);
                gameManager->addAction(newAction);
                QString moveDescription = QString("Placed %1 troops on %2").arg(troopsToAdd).arg(
                    QString::fromStdString(selected_vertex->label()));
                QListWidgetItem* item = new QListWidgetItem(moveDescription);
                item->setData(Qt::UserRole, QVariant::fromValue(selected_vertex->id()));
                item->setData(Qt::UserRole + 1, QVariant(troopsToAdd));
                item->setData(Qt::UserRole + 2, "Place");
                item->setData(Qt::UserRole + 3, newAction.id);
                moveList->addItem(item);
            }
            armyManager.decreaseAvailableTroops(troopsToAdd);
            selected_vertex->newRecruits += troopsToAdd;
            selected_vertex->army.setSoldiers(selected_vertex->army.getSoldiers() + troopsToAdd);
            characterWidget->setArmyText(armyManager.totalTroops,armyManager.maxTroops);
        }
    }
}

void ClientWindow::onInfoButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
    selectedLayer=nullptr;
    gameManager->clearTemporaryArrows();
}

void ClientWindow::onMoveButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::onPlaceButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
    selectedLayer=nullptr;
    gameManager->clearTemporaryArrows();
}

void ClientWindow::setActiveButton(QPushButton* clickedButton) {
    QString defaultStyle =
        "QPushButton { "
        "   background-color: #3A5F0B; "
        "   border-radius: 15px; "
        "   color: white;   "
        "   border: 2px solid #2E4600; "
        "   font-size: 10pt; "
        "   padding: 5px;   "
        "   border-image: none; "
        "} "
        "QPushButton:hover { "
        "   background-color: #4F7942; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2E4600; "
        "} ";

    QString activeStyle =
        "QPushButton { "
        "   background-color: #2E4600; "
        "   border-image: none; "
        "} ";

    if (activeButton) {
        activeButton->setStyleSheet(defaultStyle);
    }

    if (clickedButton) {
        clickedButton->setStyleSheet(activeStyle);
        activeButton = clickedButton;
    }
}

void ClientWindow::clearExplosions()
{
    gameManager->clearExplosions();
}

void ClientWindow::keyPressEvent(QKeyEvent *event) {
    if (isPauseMenuActive) {
        // Ignoriši pritisak tastera ESC dok je pause meni aktivan
        if (event->key() == Qt::Key_Escape) {
            event->ignore(); // Ignoriše ESC
        }
    } else {
        // Ako meni nije aktivan, normalno procesiraj ESC
        if (event->key() == Qt::Key_Escape) {
            showPauseMenu();
        }
    }
    if (event->key() == Qt::Key_I) {  // Kada pritisnete 'I', aktivirajte info dugme
        setActiveButton(infoButton);
    } else if (event->key() == Qt::Key_M) {  // Kada pritisnete 'M', aktivirajte move dugme
        setActiveButton(moveButton);
    } else if (event->key() == Qt::Key_A) {  // Kada pritisnete 'A', aktivirajte army dugme
        setActiveButton(armyButton);
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void ClientWindow::showPauseMenu() {
    if (findChild<QWidget*>("PauseMenuOverlay")) {
        findChild<QWidget*>("PauseMenuOverlay")->deleteLater();
        return;
    }

    QWidget *overlay = new QWidget(this);
    overlay->setObjectName("PauseMenuOverlay");
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    overlay->setGeometry(this->rect());

    overlay->setAttribute(Qt::WA_TransparentForMouseEvents, false); // Sprečava da overlay bude transparentan za mišićne događaje
    overlay->setFocusPolicy(Qt::StrongFocus); // Omogućava da overlay primi fokus

    QWidget *menuWidget = new QWidget(overlay);
    if (gameManager->ClientId == 1) {
        menuWidget->setStyleSheet(
            "background-color: rgba(74, 47, 47, 220); "
            "border-image: url(:/resources/border1.png) 40  stretch; "
            "border-width: 20px; "
            "padding: 10px; "
            );
    } else {
        menuWidget->setStyleSheet(
            "background-color: rgba(3, 66, 5, 220); "
            "border-image: url(:/resources/border1.png) 40  stretch; "
            "border-width: 20px; "
            "padding: 10px; "
            );
    }
    menuWidget->setFixedSize(550, 600);
    menuWidget->move((width() - menuWidget->width()) / 2, (height() - menuWidget->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setContentsMargins(60, 20, 60, 150);
    layout->setSpacing(20);

    QLabel *label = new QLabel("Game Paused");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(16);
    label->setFont(font);
    label->setStyleSheet("color: #FFD700;border-image: none;background-color:transparent;");
    layout->addWidget(label);

    QPushButton *continueButton = new QPushButton("Continue Game");
    QPushButton *saveButton = new QPushButton("Save Game");
    QPushButton *loadGame = new QPushButton("Load Game");
    QPushButton *optionsButton = new QPushButton("Options");
    QPushButton *quitButton = new QPushButton("Quit Game");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: #2E4600; "
        "   border-radius: 10px; "
        "   border: 2px solid #FFD700; "
        "   color: white; "
        "   padding: 10px;"
        "   border-image: none;"
        "} "
        "QPushButton:hover { "
        "   background-color: #4F7942; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2E4600; "
        "} ";
    continueButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);
    loadGame->setStyleSheet(buttonStyle);
    optionsButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(
        "QPushButton { "
        "   background-color: #8B0000;"
        "   border-radius: 10px; "
        "   border: 2px solid #FFD700; "
        "   color: white; "
        "   padding: 10px;"
        "   border-image: none;"
        "} "
        "QPushButton:hover { "
        "   background-color: #B22222; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #5A0000; "
        "} ");

    layout->addWidget(continueButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadGame);
    layout->addWidget(optionsButton);
    layout->addWidget(quitButton);

    connect(continueButton, &QPushButton::clicked, overlay, &QWidget::deleteLater);
    connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (gameManager) {
            gameManager->saveGame();
            CustomMessageBox::showMessage("Game saved successfully!", this);
        }
    });
    connect(loadGame, &QPushButton::clicked, this, [this](){
        if (gameManager){
            gameManager->loadGame();
            CustomMessageBox::showMessage("Game loaded successfully!", this);
        }
    });
    connect(optionsButton, &QPushButton::clicked, this, [this]() {
        CustomMessageBox::showMessage("Options menu under construction.", this);
    });

    overlay->show();
}

// Funkcija za prikaz ESC prozora
void ClientWindow::showDisconnectPauseMenu() {
    // Ako već postoji overlay, nemojte ga ponovo dodavati
    if (findChild<QWidget*>("PauseMenuOverlay")) {
        return;
    }

    QWidget *overlay = new QWidget(this);
    overlay->setObjectName("PauseMenuOverlay");
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 120);");
    overlay->setGeometry(this->rect());

    // Dodajte ove linije
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents, false); // Sprečava da overlay bude transparentan za mišićne događaje
    overlay->setFocusPolicy(Qt::StrongFocus); // Omogućava da overlay primi fokus

    QWidget *menuWidget = new QWidget(overlay);
    if (gameManager->ClientId == 1) {
        menuWidget->setStyleSheet(
            "background-color: rgba(74, 47, 47, 220); "
            "border-image: url(:/resources/border1.png) 40  stretch; "
            "border-width: 20px; "
            "padding: 10px; "
            );
    } else {
        menuWidget->setStyleSheet(
            "background-color: rgba(3, 66, 5, 220); "
            "border-image: url(:/resources/border1.png) 40  stretch; "
            "border-width: 20px; "
            "padding: 10px; "
            );
    }
    menuWidget->setFixedSize(500, 550);
    menuWidget->move((width() - menuWidget->width()) / 2, (height() - menuWidget->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setContentsMargins(60, 20, 60, 150);
    layout->setSpacing(20);

    QLabel *label = new QLabel("The client has disconnected.");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(16);
    label->setFont(font);
    label->setStyleSheet("color: white;border-image: none;background-color:transparent;");
    layout->addWidget(label);

    QPushButton *saveButton = new QPushButton("Save Game");
    QPushButton *quitButton = new QPushButton("Quit Game");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: #2E4600; "
        "   border-radius: 10px; "
        "   border: 2px solid #FFD700; "
        "   color: white; "
        "   padding: 10px;"
        "   border-image: none;"
        "} "
        "QPushButton:hover { "
        "   background-color: #4F7942; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #2E4600; "
        "} ";

    saveButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(
                  "QPushButton { "
                  "   background-color: #8B0000;"
                  "   border-radius: 10px; "
                  "   border: 2px solid #FFD700; "
                  "   color: white; "
                  "   padding: 10px;"
                  "   border-image: none;"
                  "} "
                  "QPushButton:hover { "
                  "   background-color: #B22222; "
                  "} "
                  "QPushButton:pressed { "
                  "   background-color: #5A0000; "
                  "} ");

    layout->addWidget(saveButton);
    layout->addWidget(quitButton);

    connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (gameManager) {
            gameManager->saveGame();
            CustomMessageBox::showMessage("Game saved successfully!", this);
        }
    });

    isPauseMenuActive = true;

    overlay->show();
}
