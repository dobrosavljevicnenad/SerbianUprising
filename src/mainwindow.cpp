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

    // need to implement buttons here
    // and also need to implement on moveArmy to put on buffer also buffer need

    connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
    connect(ui->changePlayerButton, &QPushButton::clicked, this, &MainWindow::onChangePlayerClicked);
    connect(ui->endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChangePlayerClicked() {
    turnManager.changePlayer();
    int currentPlayer = turnManager.getCurrentPlayer();
    ui->textField->append(QString("Player %1 is now active.").arg(currentPlayer));
}

void MainWindow::onEndTurnClicked() {
    ui->textField->append(turnManager.processBuffer());
    turnManager.endTurn();
    int currentPlayer = turnManager.getCurrentPlayer();
    ui->textField->append(QString("Player %1's turn starts.").arg(currentPlayer));
}

void MainWindow::onLayerClicked(MapLayer *layer) {
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
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
                gameManager->ma.executeMove(selected_vertex,vertex,troopsToTransfer);

                selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());
                layer->setTroopCount(vertex->army.getSoldiers());
                layer->setArmyColor(vertex->army.armyType());

                std::cout << selected_vertex->army.getSoldiers() << " " << vertex->army.getSoldiers() << std::endl;
            }
        }

        selectedLayer = nullptr;
    }
}
