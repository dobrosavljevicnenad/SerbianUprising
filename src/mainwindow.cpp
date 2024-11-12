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

    //GameManager *gameManager = new GameManager(scene);
    //gameManager->initializeMap();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::highlightLayer(MapLayer *layer) {
    layer->setColor(QColor(0, 255, 0)); // Set the layer to green to highlight it
}



void MainWindow::onLayerClicked(MapLayer *layer) {
    if (selectedLayer == nullptr) {
        selectedLayer = layer;
        //QMessageBox::information(this, tr("Selected layer"), tr("The first layer is selected."));
    } else {
        if(selectedLayer == layer) {
            QMessageBox::warning(this, tr("Error"), tr("You selected the same layer. Select another layer."));
            selectedLayer = nullptr;
            return ;
        }

        bool ok;
        int maxTroops = selectedLayer->getTroopCount();
        int troopsToTransfer = QInputDialog::getInt(this, tr("Transfer Troops"), tr("Enter the number of soldiers to transfer:"), 0, 0, maxTroops, 1, &ok);
        if (ok) {
            if(troopsToTransfer > selectedLayer->getTroopCount()) {
                QMessageBox::warning(this, tr("Error"), tr("You don`t have enough troops to transfer."));
            } else {
                selectedLayer->setTroopCount(selectedLayer->getTroopCount() - troopsToTransfer);
                layer->setTroopCount(layer->getTroopCount() + troopsToTransfer);

                //QMessageBox::information(this, tr("Tranfer successful."), tr("%1 soldiers were transferred."));
            }
        }

        selectedLayer = nullptr;
    }
}
