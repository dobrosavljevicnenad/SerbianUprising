#ifndef ADDARMYMANAGER_H
#define ADDARMYMANAGER_H

class AddArmyManager
{
public:
    AddArmyManager();

    void addTerritory();
    void removeTerritory();
    void addRegion();
    void removeRegion();
    void addCity();
    void removeCity();

    int calculateTotalTroops();
    void decreaseAvailableTroops(int troops);
    void endTurn();

private:
    int territoryCount;
    int regionCount;
    int cityCount;
    int totalTroops = 5;
    int territoryBonus = 1;
    int regionMultiplier = 2;
    int cityBonus = 5;
};

#endif // ADDARMYMANAGER_H
