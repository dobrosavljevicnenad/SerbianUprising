#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
    , selectedLayer(nullptr)
    , moveList(new QListWidget())
{
    ui->setupUi(this);

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

    QPushButton *changePlayerButton = new QPushButton("Change Player");
    QPushButton *endTurnButton = new QPushButton("End Turn");

    QGraphicsProxyWidget *changePlayerButtonProxy = scene->addWidget(changePlayerButton);
    QGraphicsProxyWidget *endTurnButtonProxy = scene->addWidget(endTurnButton);
    QGraphicsProxyWidget *textFieldProxy = scene->addWidget(moveList);

    changePlayerButtonProxy->setZValue(1);
    endTurnButtonProxy->setZValue(1);
    textFieldProxy->setZValue(1);

    changePlayerButtonProxy->resize(QSize(70,50));
    changePlayerButtonProxy->setPos(scene->width()- changePlayerButton->size().width(), 00);

    endTurnButtonProxy->resize(QSize(70,60));
    endTurnButtonProxy->setPos(0, 0);
    textFieldProxy->setPos(scene->width()-textFieldProxy->size().width(), 50);


    moveList->addItem(QString("Player %1 on turn:").arg(gameManager->turn.getCurrentPlayerId()));

    connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
    connect(changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
    connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
    connect(moveList, &QListWidget::itemClicked, this, &MainWindow::onMoveClicked);

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

    //std::cout << "Move with ID " << actionId << " removed.\n";

}

void MainWindow::onChangePlayerClicked() {
    gameManager->turn.changePlayer();

    int currentPlayer = gameManager->turn.getCurrentPlayerId();
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

    //now we need to update all graphical componenets of our project aka layers
    gameManager->updateLayersGraphics();
    moveList->clear();

    // Optionally, update the UI to show that the turn has ended
    QMessageBox::information(this, tr("Turn Ended"), tr("The turn has ended. Buffers have been cleared."));
}

void MainWindow::onLayerClicked(MapLayer *layer) {
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
        graph::Vertex* selected_vertex = gameManager->layerToVertex[selectedLayer];
        if(selected_vertex->player.getPlayerId() != gameManager->turn.getCurrentPlayerId()){
            QMessageBox::warning(this, tr("Error"), tr("You selected the enemy layer. Select another layer."));
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
        if (maxTroops <= 0) {
            QMessageBox::warning(this, tr("Error"), tr("No troops available to transfer from the selected layer."));
            selectedLayer = nullptr;
            return;
        }

        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            if(troopsToTransfer > selected_vertex->army.getSoldiers()) {
                QMessageBox::warning(this, tr("Error"), tr("You don`t have enough troops to transfer."));
            } else {
                //Action
                ActionType type = (selected_vertex->army.armyType() == vertex->army.armyType()) ? ActionType::MOVE_ARMY : ActionType::ATTACK;
                int pid = gameManager->turn.getCurrentPlayerId();
                int source = selected_vertex->id();
                int target = vertex->id();
                Action newAction(type, pid, source,target, troopsToTransfer);


                //TODO
                //trooptotransfer moramo se implementirati full - trooptotransfer ali u cvoru
                selected_vertex->army.setSoldiers(maxTroops - troopsToTransfer);
                selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());
                std::cout << selectedLayer << std::endl;
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
        }

        selectedLayer = nullptr;
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
