#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gui/maplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new MapScene(this))
{
    ui->setupUi(this);

    MapScene *scene = new MapScene(this);

    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    MapLayer *baseLayer = new MapLayer(":/resources/ceo.png",false);
    scene->addLayer(baseLayer);

    MapLayer *territoryGornji = new MapLayer(":/resources/gornji.png",true);
    MapLayer *territoryDonjiLevi = new MapLayer(":/resources/donji_l.png",true);
    MapLayer *territoryDonjiDesni = new MapLayer(":/resources/donji_d.png",true);

    territoryGornji->setPos(baseLayer->pos());
    territoryDonjiLevi->setPos(baseLayer->x()+8, baseLayer->y() + territoryGornji->boundingRect().height()-10);
    territoryDonjiDesni->setPos(baseLayer->x() + territoryDonjiLevi->boundingRect().width()+7, baseLayer->y() + territoryGornji->boundingRect().height());

    territoryGornji->setTroopCount(50);
    territoryDonjiLevi->setTroopCount(30);
    territoryDonjiDesni->setTroopCount(20);

    scene->addLayer(territoryGornji);
    scene->addLayer(territoryDonjiLevi);
    scene->addLayer(territoryDonjiDesni);

    scene->addItem(territoryGornji);
    scene->addItem(territoryDonjiLevi);
    scene->addItem(territoryDonjiDesni);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::highlightLayer(MapLayer *layer) {
    layer->setColor(QColor(0, 255, 0)); // Set the layer to green to highlight it
}
