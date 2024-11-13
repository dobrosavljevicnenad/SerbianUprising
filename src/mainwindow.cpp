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

    connect(gameManager, &GameManager::layerClicked, this, &MainWindow::onLayerClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
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
                selected_vertex->army.setSoldiers(selected_vertex->army.getSoldiers() - troopsToTransfer);
                selectedLayer->setTroopCount(selected_vertex->army.getSoldiers());
                vertex->army.setSoldiers(vertex->army.getSoldiers() + troopsToTransfer);
                layer->setTroopCount(vertex->army.getSoldiers());

                std::cout << selected_vertex->army.getSoldiers() << " " << vertex->army.getSoldiers() << std::endl;
            }
        }

        selectedLayer = nullptr;
    }
}
