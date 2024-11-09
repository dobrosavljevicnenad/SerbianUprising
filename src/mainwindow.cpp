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

    MapLayer *baseLayer = new MapLayer(":/resources/base.png",false);
    scene->addLayer(baseLayer);

    graph::Graph g;
    MapLayer *layer1 = new MapLayer(":resources/Layer2.png", true);
    graph::Vertex v1 = g.insert_vertex(layer1->troopText->pos(), "layer1", layer1);

    MapLayer *layer2 = new MapLayer(":/resources/Layer3.png", true);
    graph::Vertex v2 = g.insert_vertex(layer2->troopText->pos(), "layer2", layer2);

    MapLayer *layer3 = new MapLayer(":/resources/Layer4.png", true);
    graph::Vertex v3 = g.insert_vertex(layer3->troopText->pos(), "layer3", layer3);

    MapLayer *layer4 = new MapLayer(":/resources/Layer5.png", true);
    graph::Vertex v4 = g.insert_vertex(layer4->troopText->pos(), "layer4", layer4);

    MapLayer *layer5 = new MapLayer(":/resources/Layer6.png", true);
    graph::Vertex v5 = g.insert_vertex(layer5->troopText->pos(), "Layer5", layer5);

    MapLayer *layer6 = new MapLayer(":/resources/Layer7.png", true);
    graph::Vertex v6 = g.insert_vertex(layer6->troopText->pos(), "Layer6", layer6);

    MapLayer *layer7 = new MapLayer(":/resources/Layer8.png", true);
    graph::Vertex v7 = g.insert_vertex(layer7->troopText->pos(), "Layer7", layer7);

    MapLayer *layer8 = new MapLayer(":/resources/Layer9.png", true);
    graph::Vertex v8 = g.insert_vertex(layer8->troopText->pos(), "Layer8", layer8);

    MapLayer *layer9 = new MapLayer(":/resources/Layer10.png", true);
    graph::Vertex v9 = g.insert_vertex(layer9->troopText->pos(), "Layer9", layer9);

    MapLayer *layer10 = new MapLayer(":/resources/Layer11.png", true);
    graph::Vertex v10 = g.insert_vertex(layer10->troopText->pos(), "Layer10", layer10);

    MapLayer *layer11 = new MapLayer(":/resources/Layer12.png", true);
    graph::Vertex v11 = g.insert_vertex(layer11->troopText->pos(), "Layer11", layer11);

    MapLayer *layer12 = new MapLayer(":/resources/Layer13.png", true);
    graph::Vertex v12 = g.insert_vertex(layer12->troopText->pos(), "Layer12", layer12);

    MapLayer *layer13 = new MapLayer(":/resources/Layer14.png", true);
    graph::Vertex v13 = g.insert_vertex(layer13->troopText->pos(), "Layer13", layer13);

    g.insert_edge(v1,v3,1.0);
    g.insert_edge(v1,v5,1.0);
    g.insert_edge(v1,v11,1.0);
    g.insert_edge(v2,v3,1.0);
    g.insert_edge(v2,v4,1.0);
    g.insert_edge(v4,v3,1.0);
    g.insert_edge(v5,v3,1.0);
    g.insert_edge(v4,v6,1.0);
    g.insert_edge(v5,v6,1.0);
    g.insert_edge(v5,v9,1.0);
    g.insert_edge(v5,v10,1.0);
    g.insert_edge(v5,v11,1.0);
    g.insert_edge(v6,v7,1.0);
    g.insert_edge(v6,v8,1.0);
    g.insert_edge(v6,v9,1.0);
    g.insert_edge(v8,v7,1.0);
    g.insert_edge(v8,v9,1.0);
    g.insert_edge(v8,v13,1.0);
    g.insert_edge(v9,v13,1.0);
    g.insert_edge(v9,v10,1.0);
    g.insert_edge(v10,v13,1.0);
    g.insert_edge(v10,v11,1.0);
    g.insert_edge(v10,v12,1.0);
    g.insert_edge(v11,v13,1.0);
    g.insert_edge(v12,v13,1.0);

    print_connections(g, v1);
    print_connections(g, v2);
    print_connections(g, v3);
    print_connections(g, v4);
    print_connections(g, v5);
    print_connections(g, v6);
    print_connections(g, v7);
    print_connections(g, v8);
    print_connections(g, v9);
    print_connections(g, v10);
    print_connections(g, v11);
    print_connections(g, v12);
    print_connections(g, v13);

    layer1->setPos(baseLayer->pos().x()+633,baseLayer->pos().x()+251);
    layer2->setPos(baseLayer->pos().x()+287,baseLayer->pos().x()+400);
    layer3->setPos(baseLayer->pos().x()+446,baseLayer->pos().x()+291);
    layer4->setPos(baseLayer->pos().x()+261,baseLayer->pos().x()+268);
    layer5->setPos(baseLayer->pos().x()+378,baseLayer->pos().x()+186);
    layer6->setPos(baseLayer->pos().x()+223,baseLayer->pos().x()+130);
    layer7->setPos(baseLayer->pos().x()+154,baseLayer->pos().x()+98);
    layer8->setPos(baseLayer->pos().x()+181,baseLayer->pos().x()+27);
    layer9->setPos(baseLayer->pos().x()+359,baseLayer->pos().x()+85);
    layer10->setPos(baseLayer->pos().x()+529,baseLayer->pos().x()+76);
    layer11->setPos(baseLayer->pos().x()+706,baseLayer->pos().x()+56);
    layer12->setPos(baseLayer->pos().x()+627,baseLayer->pos().x()+36);
    layer13->setPos(baseLayer->pos().x()+379,baseLayer->pos().x()+35);


    layer1->setTroopCount(1);
    layer2->setTroopCount(2);
    layer3->setTroopCount(3);
    layer4->setTroopCount(4);
    layer5->setTroopCount(5);
    layer6->setTroopCount(6);
    layer7->setTroopCount(7);
    layer8->setTroopCount(8);
    layer9->setTroopCount(9);
    layer10->setTroopCount(10);
    layer11->setTroopCount(11);
    layer13->setTroopCount(13);

    scene->addLayer(layer1);
    scene->addLayer(layer2);
    scene->addLayer(layer3);
    scene->addLayer(layer4);
    scene->addLayer(layer5);
    scene->addLayer(layer6);
    scene->addLayer(layer7);
    scene->addLayer(layer8);
    scene->addLayer(layer9);
    scene->addLayer(layer10);
    scene->addLayer(layer11);
    scene->addLayer(layer12);
    scene->addLayer(layer13);


    scene->addItem(layer1);
    scene->addItem(layer2);
    scene->addItem(layer3);
    scene->addItem(layer4);
    scene->addItem(layer5);
    scene->addItem(layer6);
    scene->addItem(layer7);
    scene->addItem(layer8);
    scene->addItem(layer9);
    scene->addItem(layer10);
    scene->addItem(layer11);
    scene->addItem(layer12);
    scene->addItem(layer13);


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
