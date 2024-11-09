#include "mainwindow.h"
#include "./ui_mainwindow.h"


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
    graph::Graph g;
    MapLayer *territoryGornji = new MapLayer(":/resources/gornji.png",true);//ubaci

    Army army(40, ArmyType::HAJDUK);

    Territory territory(TerrainType::MOUNTAIN);

    Player player(1, ArmyType::HAJDUK);

    graph::Vertex v1 = g.insert_vertex(territoryGornji->troopText->pos(), "Gornji_layer",territoryGornji, territory, army, player);

    MapLayer *territoryDonjiLevi = new MapLayer(":/resources/donji_l.png",true);//ubaci
    graph::Vertex v2 = g.insert_vertex(territoryDonjiLevi->troopText->pos(), "DonjiLevi",territoryDonjiLevi, territory, army, player);

    MapLayer *territoryDonjiDesni = new MapLayer(":/resources/donji_d.png",true);//ubaci
    graph::Vertex v3 = g.insert_vertex(territoryDonjiDesni->troopText->pos(), "DonjiDesni",territoryDonjiDesni, territory, army, player);

    g.insert_edge(v1,v2,1.0);
    g.insert_edge(v1,v3,1.0);
    g.insert_edge(v2,v3,1.0);

    print_connections(g, v1);
    print_connections(g, v2);
    print_connections(g, v3);

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

void MainWindow::print_connections(const graph::Graph &g, const graph::Vertex &vertex) {
    auto neighbors = g.neighbors(vertex);  // Assuming neighbors() returns a vector of vertices connected by edges.
    std::cout << "Vertex " << vertex.id() << " is connected to: ";
    for (const auto &neighbor : neighbors) {
        std::cout << neighbor.id() << " ";
    }
    std::cout << std::endl;
}
