#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
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
    QString moveDetails = item->text();

    QMessageBox::information(this, tr("Move Details"), tr("You clicked on: %1").arg(moveDetails));

    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) {
        return;
    }

    int actionId = data.toInt();

    gameManager->turn.removeActionById(actionId);

    delete item;

    std::cout << "Move with ID " << actionId << " removed.\n";

}

void MainWindow::onChangePlayerClicked() {

    gameManager->turn.changePlayer();
    //textField->append(QString("Player %1 is now active.").arg(currentPlayer));
    updateMoveList();
    int currentPlayer = gameManager->turn.getCurrentPlayerId();
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
    //ui->textField->append(gameManager->turn.bufferToString());
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
        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            if(troopsToTransfer > selected_vertex->army.getSoldiers()) {
                QMessageBox::warning(this, tr("Error"), tr("You don`t have enough troops to transfer."));
            } else {
                ActionType type = (selected_vertex->army.armyType() == vertex->army.armyType()) ? ActionType::MOVE_ARMY : ActionType::ATTACK;
                int pid = gameManager->turn.getCurrentPlayerId();
                int source = selected_vertex->id();
                int target = vertex->id();
                Action newAction(type, pid, source,target, troopsToTransfer);
                gameManager->turn.addAction(pid, newAction);

                //TODO
                //trooptotransfer moramo se implementirati full - trooptotransfer ali u cvoru
                selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());
                layer->setTroopCount(vertex->army.getSoldiers());
                layer->setArmyColor(vertex->army.armyType());
                gameManager->drawArrow(selectedLayer, layer, troopsToTransfer);

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

void MainWindow::updateMoveList() {
    moveList->clear();

    const auto& buffer = gameManager->turn.getPlayerBuffer(gameManager->turn.getCurrentPlayerId());

    for (const auto& action : buffer) {
        QString moveDescription = gameManager->turn.GetCurrentAction(action);
        QListWidgetItem* item = new QListWidgetItem(moveDescription, moveList);

        // Store the action ID in the item's data for later retrieval
        item->setData(Qt::UserRole, action.id);

        // Add the item to the list
        moveList->addItem(item);
    }
}
