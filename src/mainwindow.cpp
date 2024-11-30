#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./Interface/gamemenu.h"

GameMenu *gameMenu;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
    , selectedLayer(nullptr)
    , moveList(new QListWidget())
{
    ui->setupUi(this);

    gameMenu = new GameMenu(this);
    setCentralWidget(gameMenu);

    //MapScene *scene = new MapScene(this);

    view = new QGraphicsView(scene, this);
    //setCentralWidget(view);

    gameManager = new GameManager(scene);

    connect(gameMenu, &GameMenu::startGame, this, [this]() {
        MapScene *scene = new MapScene(this);

        view = new QGraphicsView(scene, this);
        setCentralWidget(view);

        gameManager = new GameManager(scene);
        gameManager->initializeMap();

        mediaPlayer = new QMediaPlayer();
        audioOutput = new QAudioOutput();

        mediaPlayer->setAudioOutput(audioOutput);
        audioOutput->setVolume(0.0);
        mediaPlayer->setSource(QUrl::fromLocalFile("../../resources/music/Hajduk.mp3"));

        mediaPlayer->play();
        QEventLoop loop;
        QTimer::singleShot(100, &loop, &QEventLoop::quit);
        loop.exec();

        QWidget* layoutContainer = new QWidget();

        QVBoxLayout* mainLayout = new QVBoxLayout();

        QLabel* headerLabel = new QLabel("Turn 1");
        QFont font = headerLabel->font();
        font.setBold(true);
        font.setPointSize(14);
        headerLabel->setFont(font);
        headerLabel->setAlignment(Qt::AlignCenter);
        this->headerLabel = headerLabel;

        mainLayout->addWidget(headerLabel);

        QHBoxLayout* buttonRowLayout = new QHBoxLayout();

        QPushButton* infoButton = new QPushButton("Info");
        QPushButton* moveButton = new QPushButton("Move");
        this->moveButton = moveButton;
        QPushButton* armyButton = new QPushButton("Army");
        this->armyButton = armyButton;

        buttonRowLayout->addWidget(infoButton);
        buttonRowLayout->addWidget(moveButton);
        buttonRowLayout->addWidget(armyButton);

        mainLayout->addLayout(buttonRowLayout);

        QPushButton *endTurnButton = new QPushButton("End Turn");

        mainLayout->addWidget(endTurnButton);

        layoutContainer->setLayout(mainLayout);

        QGraphicsProxyWidget* layoutProxy = scene->addWidget(layoutContainer);

        layoutProxy->setPos(5, 5);

        buttonRowLayout->setSpacing(5);
        buttonRowLayout->setContentsMargins(0, 0, 0, 0);

        mainLayout->setSpacing(5);
        mainLayout->setContentsMargins(5, 5, 5, 5);

        layoutContainer->setStyleSheet("background-color: transparent;");

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

        QPushButton *changePlayerButton = new QPushButton("Change Player");

        QGraphicsProxyWidget *changePlayerButtonProxy = scene->addWidget(changePlayerButton);
        QGraphicsProxyWidget *textFieldProxy = scene->addWidget(moveList);

        changePlayerButtonProxy->setPos(scene->width()- changePlayerButton->size().width(), 00);

        textFieldProxy->setPos(scene->width()-textFieldProxy->size().width(), 50);

        changePlayerButtonProxy->setZValue(1);
        textFieldProxy->setZValue(1);

        moveList->addItem(QString("Player %1 on turn:").arg(gameManager->turn.getCurrentPlayerId()));

        connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
        connect(changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
        connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
        connect(moveList, &QListWidget::itemClicked, this, &MainWindow::onMoveClicked);
        connect(infoButton, &QToolButton::clicked, this, &MainWindow::onInfoButtonClicked);
        connect(moveButton, &QToolButton::clicked, this, &MainWindow::onMoveButtonClicked);
        connect(armyButton, &QToolButton::clicked, this, &MainWindow::onPlaceButtonClicked);

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

void MainWindow::onMoveClicked(QListWidgetItem* item) {
    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) {
        return;
    }

    int actionId = data.toInt();
    gameManager->turn.removeActionById(actionId);
    gameManager->removeArrowByActionId(actionId);

    delete item;
}

void MainWindow::onChangePlayerClicked() {
    gameManager->turn.changePlayer();

    int currentPlayer = gameManager->turn.getCurrentPlayerId();
    gameManager->updateLayersId(currentPlayer);

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

            //selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
            //selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());

            selectedLayer->setTroopCount(maxTroops - troopsToTransfer);

            gameManager->drawArrow(selectedLayer, layer, troopsToTransfer, newAction.id);
            gameManager->turn.addAction(pid, newAction);

            //TODO
            //ONLY HIGHLIGHT NEIGHBOR PROVINCE WHEN PRESSED AND ALSO
            //DON'T ALLOW CLICKS ON NOT NEIGHBOUR PROVINCE OF FIRST CLICKED
            //buffer and textfield
            QString move = gameManager->turn.GetCurrentAction(newAction);
            QListWidgetItem* item = new QListWidgetItem(move);
            item->setData(Qt::UserRole, newAction.id);
            moveList->addItem(item);
        }
        selectedLayer = nullptr;
    }
}

void MainWindow::handlePlaceArmy(MapLayer* layer){
    int currentPlayerId = gameManager->turn.getCurrentPlayerId();
    AddArmyManager& armyManager = gameManager->getArmyManager(currentPlayerId);

    int maxTroops = armyManager.calculateTotalTroops();

    graph::Vertex* selected_vertex = gameManager->layerToVertex[layer];

    bool ok;
    int troopsToAdd = QInputDialog::getInt(this, tr("Place Army"),
                                           tr("Enter the number of troops to place:"), 1, 1, 100, 1, &ok);
    if (ok && troopsToAdd > 0) {
        layer->setTroopCount(layer->getTroopCount() + troopsToAdd);

        armyManager.decreaseAvailableTroops(troopsToAdd);

        QString moveDescription = QString("Placed %1 troops on %2").arg(troopsToAdd).arg(selected_vertex->id());
        QListWidgetItem* item = new QListWidgetItem(moveDescription);
        item->setData(Qt::UserRole, QVariant::fromValue(selected_vertex->id()));
        item->setData(Qt::UserRole + 1, QVariant(troopsToAdd));
        moveList->addItem(item);
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
