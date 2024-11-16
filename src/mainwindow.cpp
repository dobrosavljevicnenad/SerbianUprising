#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
    , selectedLayer(nullptr)
{
    ui->setupUi(this);

    MapScene *scene = new MapScene(this);

    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    gameManager = new GameManager(scene);
    gameManager->initializeMap();
    QPushButton *changePlayerButton = new QPushButton("Change Player");
    QPushButton *endTurnButton = new QPushButton("End Turn");
    QTextBrowser *textField = new QTextBrowser();

    QGraphicsProxyWidget *changePlayerButtonProxy = scene->addWidget(changePlayerButton);
    QGraphicsProxyWidget *endTurnButtonProxy = scene->addWidget(endTurnButton);
    QGraphicsProxyWidget *textFieldProxy = scene->addWidget(textField);

    changePlayerButtonProxy->setZValue(1);
    endTurnButtonProxy->setZValue(1);
    textFieldProxy->setZValue(1);

    changePlayerButtonProxy->setPos(850, 10);
    endTurnButtonProxy->setPos(1000, 10);
    textFieldProxy->setPos(840, 50);


    // and also need to implement on moveArmy to put on buffer also buffer need

    connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
    connect(changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
    connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChangePlayerClicked() {
    int currentPlayer = gameManager->turn.getCurrentPlayerId();

    gameManager->turn.changePlayer();
    currentPlayer = gameManager->turn.getCurrentPlayerId();
    std::cout << std::endl;

    //textField->append(QString("Player %1 is now active.").arg(currentPlayer));
}


void MainWindow::onEndTurnClicked() {
    //ui->textField->append(gameManager->turn.bufferToString());
    gameManager->turn.executeTurn();
    //now we need to update all graphical componenets of our project aka layers
    gameManager->updateLayersGraphics();
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

                std::cout << selected_vertex->army.getSoldiers() << " " << vertex->army.getSoldiers() << std::endl;
            }
        }

        selectedLayer = nullptr;
    }
}
