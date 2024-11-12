#include "gamemanager.h"

GameManager::GameManager(QGraphicsScene* scene) : scene(scene)  {}

void GameManager::initializeMap(){
    /*
    print_connections(g, v1);
    print_connections(g, v2);
    print_connections(g, v3);


    auto neigh = g.neighbors(v1);
    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }

    v2->army.setSoldiers(10);

    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }

    territoryGornji->setTroopCount(50);
    territoryDonjiLevi->setTroopCount(30);
    territoryDonjiDesni->setTroopCount(20);

    scene->addLayer(territoryGornji);
    scene->addLayer(territoryDonjiLevi);
    scene->addLayer(territoryDonjiDesni);

    scene->addItem(territoryGornji);
    scene->addItem(territoryDonjiLevi);
    scene->addItem(territoryDonjiDesni);

    connect(territoryGornji, &MapLayer::layerClicked, this, &MainWindow::onLayerClicked);
    connect(territoryDonjiLevi, &MapLayer::layerClicked, this, &MainWindow::onLayerClicked);
    connect(territoryDonjiDesni, &MapLayer::layerClicked, this, &MainWindow::onLayerClicked);
    */
    MapLayer *baseLayer = new MapLayer(":/resources/base.png", false);
    scene->addItem(baseLayer);

    std::vector<MapLayer*> layers = {
        new MapLayer(":/resources/Layer2.png", true),
        new MapLayer(":/resources/Layer3.png", true),
        new MapLayer(":/resources/Layer4.png", true),
        new MapLayer(":/resources/Layer5.png", true),
        new MapLayer(":/resources/Layer6.png", true),
        new MapLayer(":/resources/Layer7.png", true),
        new MapLayer(":/resources/Layer8.png", true),
        new MapLayer(":/resources/Layer9.png", true),
        new MapLayer(":/resources/Layer10.png", true),
        new MapLayer(":/resources/Layer11.png", true),
        new MapLayer(":/resources/Layer12.png", true),
        new MapLayer(":/resources/Layer13.png", true),
        new MapLayer(":/resources/Layer14.png", true),
    };

    std::vector<std::pair<int, int>> positions = {
        {633, 251}, {287, 400}, {446, 291}, {261, 268}, {378, 186},
        {223, 130}, {154, 98},  {181, 27},  {359, 85},  {529, 76},
        {706, 56},  {627, 36},  {379, 35}
    };

    std::vector<std::string> labels = {"Layer2", "Layer3", "Layer4", "Layer5","Layer6",
                                       "Layer7", "Layer8", "Layer9","Layer10", "Layer11", "Layer12",
                                       "Layer13", "Layer14" };

    Territory defaultTerritory(TerrainType::MOUNTAIN);
    Army defaultArmy(50, ArmyType::HAJDUK);
    Player defaultPlayer(1, ArmyType::HAJDUK);


    for (size_t i = 0; i < layers.size(); ++i) {
        addLayer(layers[i], labels[i], defaultTerritory, defaultArmy, defaultPlayer);
        layers[i]->setPos(baseLayer->pos().x() + positions[i].first,
                          baseLayer->pos().y() + positions[i].second); // Example positioning
    }


    if (layers.size() >= 13) {
        g.insert_edge(layerToVertex[layers[0]], layerToVertex[layers[2]], 1.0);
        g.insert_edge(layerToVertex[layers[0]], layerToVertex[layers[4]], 1.0);
        g.insert_edge(layerToVertex[layers[0]], layerToVertex[layers[10]], 1.0);
        g.insert_edge(layerToVertex[layers[1]], layerToVertex[layers[2]], 1.0);
        g.insert_edge(layerToVertex[layers[1]], layerToVertex[layers[3]], 1.0);
        g.insert_edge(layerToVertex[layers[3]], layerToVertex[layers[2]], 1.0);
        g.insert_edge(layerToVertex[layers[4]], layerToVertex[layers[2]], 1.0);
        g.insert_edge(layerToVertex[layers[3]], layerToVertex[layers[5]], 1.0);
        g.insert_edge(layerToVertex[layers[4]], layerToVertex[layers[5]], 1.0);
        g.insert_edge(layerToVertex[layers[4]], layerToVertex[layers[8]], 1.0);
        g.insert_edge(layerToVertex[layers[4]], layerToVertex[layers[9]], 1.0);
        g.insert_edge(layerToVertex[layers[4]], layerToVertex[layers[10]], 1.0);
        g.insert_edge(layerToVertex[layers[5]], layerToVertex[layers[6]], 1.0);
        g.insert_edge(layerToVertex[layers[5]], layerToVertex[layers[7]], 1.0);
        g.insert_edge(layerToVertex[layers[5]], layerToVertex[layers[8]], 1.0);
        g.insert_edge(layerToVertex[layers[7]], layerToVertex[layers[6]], 1.0);
        g.insert_edge(layerToVertex[layers[7]], layerToVertex[layers[8]], 1.0);
        g.insert_edge(layerToVertex[layers[7]], layerToVertex[layers[12]], 1.0);
        g.insert_edge(layerToVertex[layers[8]], layerToVertex[layers[12]], 1.0);
        g.insert_edge(layerToVertex[layers[8]], layerToVertex[layers[9]], 1.0);
        g.insert_edge(layerToVertex[layers[9]], layerToVertex[layers[12]], 1.0);
        g.insert_edge(layerToVertex[layers[9]], layerToVertex[layers[10]], 1.0);
        g.insert_edge(layerToVertex[layers[9]], layerToVertex[layers[11]], 1.0);
        g.insert_edge(layerToVertex[layers[10]], layerToVertex[layers[12]], 1.0);
        g.insert_edge(layerToVertex[layers[11]], layerToVertex[layers[12]], 1.0);
    }

}

void GameManager::addLayer(MapLayer* layer, const std::string& label, Territory territory, Army army, Player player) {
    scene->addItem(layer);
    auto vertex = g.insert_vertex(layer->troopText->pos(), label, layer, territory, army, player);
    layerToVertex[layer] = vertex;
}

void GameManager::transferTroops(MapLayer* from, MapLayer* to, int troops) {
    if (troops > 0 && troops <= from->getTroopCount()) {
        from->setTroopCount(from->getTroopCount() - troops);
        to->setTroopCount(to->getTroopCount() + troops);
    }
}

void GameManager::printConnections(graph::Vertex* vertex) {
    auto neighbors = g.neighbors(vertex);
    std::cout << "Vertex " << vertex->id() << " is connected to: ";
    for (auto &neighbor : neighbors) {
        std::cout << neighbor->id() << " ";
    }
    std::cout << std::endl;
}
