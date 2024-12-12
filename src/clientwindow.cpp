#include "clientwindow.h"
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include<QListWidget>
#include<QDebug>

ClientWindow::ClientWindow(ClientGameManager *existingGameManager,QWidget *parent)
    : QMainWindow(parent),
    scene(new MapScene(this)),
    gameManager(existingGameManager),
    selectedLayer(nullptr),
    moveList(new QListWidget())
{
    setupGame();
    setupUI();
    connectSignals();
    gameManager->initializeUI(headerLabel, endTurnButton, moveButton, infoButton,moveList,armyButton);
}

ClientWindow::~ClientWindow() {
    qDebug() << "Destroying ClientWindow...";
}

void ClientWindow::setupGame() {
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);
    gameManager->setScene(scene);
    gameManager->initializeGraphics();
}

void ClientWindow::setupUI() {
    QWidget *layoutContainer = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    QString label = QString("Turn %1").arg(gameManager->TurnId);
    headerLabel = new QLabel(label);
    QFont font = headerLabel->font();
    font.setBold(true);
    font.setPointSize(12);
    headerLabel->setFont(font);
    headerLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(headerLabel);

    QHBoxLayout *buttonRowLayout = new QHBoxLayout();
    infoButton = new QPushButton("Info");
    moveButton = new QPushButton("Move");
    armyButton = new QPushButton("Army");

    buttonRowLayout->addWidget(infoButton);
    buttonRowLayout->addWidget(moveButton);
    buttonRowLayout->addWidget(armyButton);

    mainLayout->addLayout(buttonRowLayout);

    endTurnButton = new QPushButton("End Turn");
    endTurnButton->setFixedSize(160, 30);
    endTurnButton->setStyleSheet(
        "QPushButton { "
        "   background-color: black; "
        "   color: white; "
        "   border-radius: 10px; "
        "   padding: 5px; "
        "} "
        "QPushButton:hover { "
        "   background-color: darkGray; "
        "} "
        "QPushButton:pressed { "
        "   background-color: black; "
        "}");

    mainLayout->addWidget(endTurnButton, 0, Qt::AlignCenter);

    layoutContainer->setStyleSheet("background-color: transparent;");

    buttonRowLayout->setSpacing(3);
    buttonRowLayout->setContentsMargins(0, 0, 0, 0);

    headerLabel->setStyleSheet("color: black; background-color: transparent;");

    QString buttonStyle =
        "QPushButton { "
        "   background-color: darkGray; "
        "   border-radius: 20px; "
        "} "
        "QPushButton:hover { "
        "   background-color: Green; "
        "} "
        "QPushButton:pressed { "
        "   background-color: darkGreen; "
        "} ";

    infoButton->setStyleSheet(buttonStyle);
    moveButton->setStyleSheet(buttonStyle);

    QString initStyle =
        "QPushButton { "
        "   background-color: darkGreen; "
        "   border-radius: 20px; "
        "} ";
    armyButton->setStyleSheet(initStyle);
    this->activeButton = armyButton;


    infoButton->setFixedSize(40, 40);
    moveButton->setFixedSize(40, 40);
    armyButton->setFixedSize(40, 40);

    moveList = new QListWidget();
    mainLayout->addWidget(moveList);

    layoutContainer->setLayout(mainLayout);
    QGraphicsProxyWidget *layoutProxy = scene->addWidget(layoutContainer);
    layoutProxy->setPos(2, 2);

    moveList->setFixedSize(200, 300);
    moveList->setStyleSheet(
        "QListWidget { "
        "   background-color: darkGray; "
        "   border: 1px solid black; "
        "   border-radius: 5px; "
        "} "
        );
    QGraphicsProxyWidget *moveListProxy = scene->addWidget(moveList);
    moveListProxy->setPos(scene->width() - moveList->width() - 10, 10);
    moveListProxy->setZValue(1);

    moveList->addItem(QString("Player %1 on turn:").arg(gameManager->ClientId));
}

void ClientWindow::connectSignals() {
    connect(gameManager, &ClientGameManager::layerClicked, this, &ClientWindow::onLayerClicked);
    connect(endTurnButton, &QPushButton::clicked, this, &ClientWindow::onEndTurnClicked);
    connect(moveList, &QListWidget::itemClicked, this, &ClientWindow::onMoveClicked);
    connect(infoButton, &QPushButton::clicked, this, &ClientWindow::onInfoButtonClicked);
    connect(moveButton, &QPushButton::clicked, this, &ClientWindow::onMoveButtonClicked);
    connect(armyButton, &QPushButton::clicked, this, &ClientWindow::onPlaceButtonClicked);
}

// void ClientWindow::onEndTurnClicked() {
//     //headerLabel->setText(QString("Turn %1").arg(gameManager->turn.getTurn()));
//     //headerLabel->setText(QString("Turn 1"));
//
//     //gameManager->getArmyManager(1).endTurn();
//     //gameManager->getArmyManager(2).endTurn();
//     //moveList->addItem(QString("Player %1 on turn:").arg(gameManager->turn.getCurrentPlayerId()));
//     gameManager->actionBuffer.clear();
// }

void ClientWindow::onEndTurnClicked() {
    if (!gameManager) {
        qWarning() << "Game manager is not available!";
        return;
    }

    QVector<Action> actions = gameManager->actionBuffer;
    int clientId = gameManager->ClientId;

    gameManager->EndTurnClicked(actions, clientId);
    gameManager->disableInteractions();
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
            gameManager->layerToVertex[layer]->army.setSoldiers(
            gameManager->layerToVertex[layer]->army.getSoldiers() - troopsToRemove);
            layer->setTroopCount(layer->getTroopCount() - troopsToRemove);
        }

        delete moveList->takeItem(moveList->row(item));
    }
}

void ClientWindow::onLayerClicked(MapLayer* layer) {

    if (activeButton == moveButton) {
        handleMoveArmy(layer);
    } else if (activeButton == armyButton) {
        handlePlaceArmy(layer);
    } else {
        QMessageBox::warning(this, tr("Unknown Action"), tr("This action is not supported."));
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
    } else {
        if(selectedLayer == layer) {
            QMessageBox::warning(this, tr("Error"), tr("You selected the same layer. Select another layer."));
            selectedLayer = nullptr;
            return ;
        }
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        graph::Vertex* vertex = gameManager->layerToVertex[layer];

        bool ok;
        int maxTroops = selected_vertex->army.getSoldiers();

        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            //Action
            ActionType type = (selected_vertex->army.armyType() == vertex->army.armyType()) ? ActionType::MOVE_ARMY : ActionType::ATTACK;
            if(selected_vertex == vertex)
                type = ActionType::PLACE_ARMY;

            int pid = gameManager->ClientId;
            int source = selected_vertex->id();
            int target = vertex->id();
            Action newAction(type, pid, source,target, troopsToTransfer);

            selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
            selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());


            gameManager->drawArrow(gameManager->ClientId,selectedLayer, layer, troopsToTransfer, newAction.id);
            gameManager->addAction(newAction);


            //TODO
            //ONLY HIGHLIGHT NEIGHBOR PROVINCE WHEN PRESSED AND ALSO
            //DON'T ALLOW CLICKS ON NOT NEIGHBOUR PROVINCE OF FIRST CLICKED
            //buffer and textfield
            QString move = gameManager->GetCurrentAction(newAction);
            QListWidgetItem* item = new QListWidgetItem(move);
            item->setData(Qt::UserRole, newAction.id);
            item->setData(Qt::UserRole + 2, "Move");
            moveList->addItem(item);
        }
        selectedLayer = nullptr;
    }
}

void ClientWindow::handlePlaceArmy(MapLayer* layer){
    int currentPlayerId = gameManager->ClientId;
    graph::Vertex* selected_vertex = gameManager->layerToVertex[layer];
    qDebug() << selected_vertex->player.getPlayerId() << "=" << gameManager->ClientId;
    if ( selected_vertex->player.getPlayerId() == currentPlayerId ) {
        AddArmyManager& armyManager = gameManager->getArmyManager();

        int maxTroops = armyManager.calculateTotalTroops();


        bool ok;
        int troopsToAdd = QInputDialog::getInt(this, tr("Place Army"),
                                               tr("Enter the number of troops to place:"), 0, 0, maxTroops, 1, &ok);
        if (ok && troopsToAdd > 0) {
            layer->setTroopCount(layer->getTroopCount() + troopsToAdd);

            int pid = gameManager->ClientId;
            int source = selected_vertex->id();
            Action newAction(ActionType::PLACE_ARMY, pid, source, 0, troopsToAdd);
            gameManager->addAction(newAction);
            armyManager.decreaseAvailableTroops(troopsToAdd);

            selected_vertex->army.setSoldiers(selected_vertex->army.getSoldiers()+troopsToAdd);

            QString moveDescription = QString("Placed %1 troops on %2").arg(troopsToAdd).arg(selected_vertex->id());
            QListWidgetItem* item = new QListWidgetItem(moveDescription);
            item->setData(Qt::UserRole, QVariant::fromValue(selected_vertex->id()));
            item->setData(Qt::UserRole + 1, QVariant(troopsToAdd));
            item->setData(Qt::UserRole + 2, "Place");
            moveList->addItem(item);
        }
    }
}


void ClientWindow::onInfoButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::onMoveButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::onPlaceButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void ClientWindow::setActiveButton(QPushButton* clickedButton) {
    QString defaultStyle =
        "QPushButton { "
        "   background-color: darkGray; "
        "   border-radius: 20px; "
        "} "
        "QPushButton:hover { "
        "   background-color: Green; "
        "} "
        "QPushButton:pressed { "
        "   background-color: darkGreen; "
        "} ";

    QString activeStyle =
        "QPushButton { "
        "   background-color: Green; "
        "   color: white; "
        "   border-radius: 20px; "
        "} ";

    if (activeButton) {
        activeButton->setStyleSheet(defaultStyle);
    }

    if (clickedButton) {
        clickedButton->setStyleSheet(activeStyle);
        activeButton = clickedButton;
    }
}

