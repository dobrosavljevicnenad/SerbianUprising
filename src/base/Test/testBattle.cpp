#include "testBattle.h"


void test_Battle() {
    Strength::instance().setBoost(ArmyType::HAJDUK, 25);
    Strength::instance().setBoost(ArmyType::JANISSARY, 0);
    graph::Graph g;

    Army army(30, ArmyType::HAJDUK);
    Army army2(50, ArmyType::JANISSARY);
    Army army3(30, ArmyType::JANISSARY);

    Terrain terrain(TerrainType::MOUNTAIN);

    Player player1(1, ArmyType::HAJDUK);
    Player player2(2, ArmyType::JANISSARY);

    MapLayer *terrainGornji = new MapLayer(":/resources/gornji.png",true);

    graph::Vertex* v1 = g.insert_vertex(terrainGornji->troopText->pos(), "Gornji_layer",terrainGornji, terrain, army, player1);
    MapLayer *terrainDonjiLevi = new MapLayer(":/resources/gornji.png",true);

    graph::Vertex* v2 = g.insert_vertex(terrainDonjiLevi->troopText->pos(), "DonjiLevi",terrainDonjiLevi, terrain, army2, player2);
    MapLayer *terrainDonjiDesni = new MapLayer(":/resources/gornji.png",true);

    graph::Vertex* v3 = g.insert_vertex(terrainDonjiDesni->troopText->pos(), "DonjiDesni",terrainDonjiDesni, terrain, army3, player2);

    g.insert_edge(v1,v2,1.0);
    g.insert_edge(v1,v3,1.0);
    g.insert_edge(v2,v3,1.0);
    Turn turnManager;
    MoveArmy ma(g,turnManager);
    std::cout << "Moving Jannisary 20 of 30 army to Hajduk army\n"<< std::endl;
    ma.executeMove(v3,v1,20);
    std::cout << "V1 " << v1->army.to_string(v1->army.armyType())<<" soldiers left: "<< v1->army.getSoldiers()
              << "\nV3 " << v3->army.to_string(v3->army.armyType())<<" soldiers left: "<< v3->army.getSoldiers()
              << "\nV2 " << v2->army.to_string(v2->army.armyType())<<" soldiers left: "<< v2->army.getSoldiers()<< std::endl;

    std::cout << "Moving Jannisary 40 of 50 army to Hajduk army\n"<< std::endl;
    ma.executeMove(v2,v1,40);
    std::cout << "V1 " << v1->army.to_string(v1->army.armyType())<<" soldiers left: "<< v1->army.getSoldiers() << " \n"
              << "V3 " << v3->army.to_string(v3->army.armyType())<<" soldiers left: "<< v3->army.getSoldiers() <<" \n"
              << "V2 " << v2->army.to_string(v2->army.armyType())<<" soldiers left: "<< v2->army.getSoldiers() <<"\n" << std::endl;

}
