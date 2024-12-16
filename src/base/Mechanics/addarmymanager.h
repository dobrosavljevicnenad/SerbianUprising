#ifndef ADDARMYMANAGER_H
#define ADDARMYMANAGER_H
#include "../graph/graph.hpp"
class AddArmyManager
{
public:
    AddArmyManager(graph::Graph* graph);

    void addTerritory(Player player);
    void removeTerritory();
    void addRegion();
    void removeRegion();
    void addCity();
    void removeCity();

    int calculateTotalTroops();
    void decreaseAvailableTroops(int troops);
    void endTurn();
    int newRecruits(Player player);

private:
    graph::Graph* graph;
    int territoryCount;
    int regionCount;
    int cityCount;
    int totalTroops = 5;
    int territoryBonus = 1;
    int regionMultiplier = 2;
    int cityBonus = 5;
};

#endif // ADDARMYMANAGER_H
