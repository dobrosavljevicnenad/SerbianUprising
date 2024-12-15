#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./Interface/gamemenu.h"
#include <qtoolbutton.h>

GameMenu *gameMenu;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
    , selectedLayer(nullptr)
{
    ui->setupUi(this);
    gameMenu = new GameMenu(this);
    setCentralWidget(gameMenu);

    view = new ZoomableGraphicsView(this);
    view->setScene(scene);

    gameManager = new GameManager(scene);

    connect(gameMenu, &GameMenu::startGame, this, [this]() {
        MapScene *scene = new MapScene(this);

        view = new ZoomableGraphicsView(this);
        view->setScene(scene);
        view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        view->setFocusPolicy(Qt::StrongFocus);
        view->setFocus();
        setCentralWidget(view);

        gameManager = new GameManager(scene);
        gameManager->initializeMap();

        //setupFixedWidgets();

        layoutContainer = new QWidget(view->viewport());
        QVBoxLayout *mainLayout = new QVBoxLayout();

        mainLayout->setSpacing(5);
        mainLayout->setContentsMargins(2, 2, 2, 2);

        // Header Label
        QString turnLabel = QString("Turn %1").arg(1);
        headerLabel = new QLabel(turnLabel);
        QFont font = headerLabel->font();
        font.setBold(true);
        font.setPointSize(12);
        headerLabel->setFont(font);
        headerLabel->setAlignment(Qt::AlignCenter);
        headerLabel->setStyleSheet("color: black; background-color: transparent;");
        mainLayout->addWidget(headerLabel);

        // Buttons Row
        QHBoxLayout *buttonRowLayout = new QHBoxLayout();
        infoButton = new QPushButton("Info");
        moveButton = new QPushButton("Move");
        armyButton = new QPushButton("Army");

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
        armyButton->setStyleSheet(
            "QPushButton { "
            "   background-color: darkGreen; "
            "   border-radius: 20px; "
            "} "
            );

        infoButton->setFixedSize(40, 40);
        moveButton->setFixedSize(40, 40);
        armyButton->setFixedSize(40, 40);
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
            "}"
            );
        mainLayout->addWidget(endTurnButton, 0, Qt::AlignCenter);
        QPushButton *changePlayerButton = new QPushButton("Change Player", layoutContainer);
        changePlayerButton->setFixedSize(160, 30);
        changePlayerButton->setStyleSheet(
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
            "}"
            );
        mainLayout->addWidget(changePlayerButton, 0, Qt::AlignCenter);

        // Move List
        moveList = new QListWidget(view->viewport());
        moveList->setFixedSize(200, 300);
        moveList->setStyleSheet(
            "QListWidget { "
            "   background-color: darkGray; "
            "   border: 1px solid black; "
            "   border-radius: 5px; "
            "}"
            );
        mainLayout->addWidget(moveList, 0, Qt::AlignCenter);

        layoutContainer->setLayout(mainLayout);

        // Set the initial position of layoutContainer
        layoutContainer->setGeometry(10, 10, 250, 400);

        // Add player change button


        // Initial move list
        moveList->addItem(QString("Player %1 on turn:").arg(gameManager->turn.getCurrentPlayerId()));

        // Connect signals
        connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
        connect(changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
        connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
        connect(moveList, &QListWidget::itemClicked, this, &MainWindow::onMoveClicked);
        connect(infoButton, &QToolButton::clicked, this, &MainWindow::onInfoButtonClicked);
        connect(moveButton, &QToolButton::clicked, this, &MainWindow::onMoveButtonClicked);
        connect(armyButton, &QToolButton::clicked, this, &MainWindow::onPlaceButtonClicked);

        // Connect scrollbars to reposition the widgets dynamically
        connect(view->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });
        connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, [this]() { repositionFixedWidgets(); });

    });

    connect(gameMenu, &GameMenu::exitGame, this, &QMainWindow::close);

    connect(gameMenu, &GameMenu::fullScreenClicked, this, [this] () {
        if(isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::repositionFixedWidgets()
{
    // Always position layoutContainer relative to the viewport
    layoutContainer->move(view->viewport()->rect().topLeft() + QPoint(10, 10));
}

void MainWindow::onMoveClicked(QListWidgetItem* item) {
    if (!item) return;

    QString actionType = item->data(Qt::UserRole + 2).toString();

    if (actionType == "Move") {
        QVariant data = item->data(Qt::UserRole);
        if (!data.isValid()) {
            return;
        }

        int actionId = data.toInt();
        gameManager->turn.removeActionById(actionId);
        gameManager->removeArrowByActionId(actionId);

        delete item;
    } else if (actionType == "Place") {
        int layerId = item->data(Qt::UserRole).toInt();
        int troopsToRemove = item->data(Qt::UserRole + 1).toInt();

        MapLayer* layer = nullptr;
        for (auto& pair : gameManager->layerToVertex) {
            if (pair->id() == layerId) {
                layer = pair->map_layer;
                break;
            }
        }


        if (layer) {
            int currentPlayerId = gameManager->turn.getCurrentPlayerId();
            AddArmyManager& armyManager = gameManager->getArmyManager(currentPlayerId);
            armyManager.decreaseAvailableTroops(-troopsToRemove);
            gameManager->layerToVertex[layer]->army.setSoldiers(gameManager->layerToVertex[layer]->army.getSoldiers()-troopsToRemove);
            layer->setTroopCount(layer->getTroopCount() - troopsToRemove);
        }

        // Remove item from the list
        delete moveList->takeItem(moveList->row(item));
    }
}



void MainWindow::onChangePlayerClicked() {
    gameManager->turn.changePlayer();

    int currentPlayer = gameManager->turn.getCurrentPlayerId();
    gameManager->updateLayersId(currentPlayer);

    gameManager->filterAndRedrawArrows(currentPlayer);

    updateMoveList(currentPlayer);
    mediaPlayer->stop();
    if(currentPlayer == 1){
        mediaPlayer->setSource(QUrl::fromLocalFile("../../resources/music/Hajduk.mp3"));
    }
    else{
        mediaPlayer->setSource(QUrl::fromLocalFile("../../resources/music/Janissary.mp3"));
    }
    std::cout << std::endl;
    mediaPlayer->play();
}

void MainWindow::onEndTurnClicked() {
    gameManager->turn.executeTurn();
    headerLabel->setText(QString("Turn %1").arg(gameManager->turn.getTurn()));

    //now we need to update all graphical componenets of our project aka layers
    gameManager->updateLayersGraphics();
    moveList->clear();
    gameManager->getArmyManager(1).endTurn();
    gameManager->getArmyManager(2).endTurn();
    updateMoveList(gameManager->turn.getCurrentPlayerId());
}

void MainWindow::onLayerClicked(MapLayer *layer) {

    if (activeButton == moveButton) {
        handleMoveArmy(layer);
    } else if (activeButton == armyButton) {
        handlePlaceArmy(layer);
    } else {
        QMessageBox::warning(this, tr("Unknown Action"), tr("This action is not supported."));
    }

}

void MainWindow::handleMoveArmy(MapLayer* layer){
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        if(selected_vertex->player.getPlayerId() != gameManager->turn.getCurrentPlayerId()){
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
            int pid = gameManager->turn.getCurrentPlayerId();
            int source = selected_vertex->id();
            int target = vertex->id();
            Action newAction(type, pid, source,target, troopsToTransfer);

            selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
            selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());

            gameManager->drawArrow(gameManager->turn.getCurrentPlayerId(),selectedLayer, layer, troopsToTransfer, newAction.id);
            gameManager->turn.addAction(pid, newAction);

            //TODO
            //ONLY HIGHLIGHT NEIGHBOR PROVINCE WHEN PRESSED AND ALSO
            //DON'T ALLOW CLICKS ON NOT NEIGHBOUR PROVINCE OF FIRST CLICKED
            //buffer and textfield
            QString move = gameManager->turn.GetCurrentAction(newAction);
            QListWidgetItem* item = new QListWidgetItem(move);
            item->setData(Qt::UserRole, newAction.id);
            item->setData(Qt::UserRole + 2, "Move");
            moveList->addItem(item);
        }
        selectedLayer = nullptr;
    }
}

void MainWindow::handlePlaceArmy(MapLayer* layer){
    int currentPlayerId = gameManager->turn.getCurrentPlayerId();
    graph::Vertex* selected_vertex = gameManager->layerToVertex[layer];

    if ( selected_vertex->player.getPlayerId() == currentPlayerId ) {
        AddArmyManager& armyManager = gameManager->getArmyManager(currentPlayerId);

        int maxTroops = armyManager.calculateTotalTroops();


        bool ok;
        int troopsToAdd = QInputDialog::getInt(this, tr("Place Army"),
                                               tr("Enter the number of troops to place:"), 0, 0, maxTroops, 1, &ok);
        if (ok && troopsToAdd > 0) {
            layer->setTroopCount(layer->getTroopCount() + troopsToAdd);

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

void MainWindow::updateMoveList(int currentPlayer) {
    moveList->clear();
    moveList->addItem(QString("Player %1 on turn:").arg(currentPlayer));

    const auto& buffer = gameManager->turn.getPlayerBuffer(currentPlayer);

    for (const auto& action : buffer) {
        QString moveDescription = gameManager->turn.GetCurrentAction(action);
        QListWidgetItem* item = new QListWidgetItem(moveDescription, moveList);

        item->setData(Qt::UserRole, action.id);

        moveList->addItem(item);
    }
}

void MainWindow::onInfoButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void MainWindow::onMoveButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void MainWindow::onPlaceButtonClicked() {
    setActiveButton(qobject_cast<QPushButton*>(sender()));
}

void MainWindow::setActiveButton(QPushButton* clickedButton) {
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
