#include "gamemanager.h"

GameManager::GameManager(QGraphicsScene* scene) : scene(scene)  {}

void GameManager::initializeMap(){
    /*
    auto neigh = g.neighbors(v1);
    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }

    v2->army.setSoldiers(10);

    for(auto n : neigh){
        std::cout << n->army.getSoldiers() << std::endl;
    }
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

    std::vector<Army> armies;
    std::vector<Player> players;
    Terrain defaultTerrain(TerrainType::MOUNTAIN);

    int numLayers = layers.size();
    for (int i = 0; i < numLayers; ++i) {
        int soldiers = 10 + std::rand() % 91;

        ArmyType type = (std::rand() % 2 == 0 ) ? ArmyType::HAJDUK : ArmyType::JANISSARY;

        armies.emplace_back(soldiers,type);

        players.emplace_back((type==ArmyType::HAJDUK) ? 1 : 2, type);
    }

    for (size_t i = 0; i < layers.size(); ++i) {
        addLayer(layers[i], labels[i], defaultTerrain, armies[i], players[i]);
        layers[i]->setPos(baseLayer->pos().x() + positions[i].first,
                          baseLayer->pos().y() + positions[i].second);

        connect(layers[i], &MapLayer::layerClicked, this, [this, layers, i]() {
            emit layerClicked(layers[i]);
        });
        layers[i]->setTroopCount(layerToVertex[layers[i]]->army.getSoldiers());
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

void GameManager::addLayer(MapLayer* layer, const std::string& label, Terrain terrain, Army army, Player player) {
    scene->addItem(layer);
    auto vertex = g.insert_vertex(layer->troopText->pos(), label, layer, terrain, army, player);
    layerToVertex[layer] = vertex;
}

void GameManager::transferTroops(MapLayer* from, MapLayer* to, int troops) {
    if (troops > 0 && troops <= from->getTroopCount()) {
        from->setTroopCount(from->getTroopCount() - troops);
        to->setTroopCount(to->getTroopCount() + troops);
    }
}

//print_connections(g, cvor);
void GameManager::printConnections(graph::Vertex* vertex) {
    auto neighbors = g.neighbors(vertex);
    std::cout << "Vertex " << vertex->id() << " is connected to: ";
    for (auto &neighbor : neighbors) {
        std::cout << neighbor->id() << " ";
    }
    std::cout << std::endl;
}
