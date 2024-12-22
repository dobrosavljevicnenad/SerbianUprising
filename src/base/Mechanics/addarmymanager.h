#ifndef ADDARMYMANAGER_H
#define ADDARMYMANAGER_H
#include "../graph/graph.hpp"
class AddArmyManager
{
public:
    AddArmyManager(graph::Graph* graph);

    void addTerritory(Player player);
    void removeTerritory();
    void addClientId(unsigned clientId);
    int calculateTotalTroops();
    void decreaseAvailableTroops(int troops);
    void endTurn();
    int newRecruits(Player player);
    int totalTroops = 5;
    void updateRegionOwnership(unsigned clientId, const QVector<Region*>& regions);

private:

    std::map<std::string, bool> regionOwnershipStatus;
    graph::Graph* graph;
    int territoryCount;
    int regionCount;
    int cityCount;
    int territoryBonus = 1;
    int regionMultiplier = 2;
    int cityBonus = 5;
};

#endif // ADDARMYMANAGER_H
