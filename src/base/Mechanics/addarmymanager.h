#ifndef ADDARMYMANAGER_H
#define ADDARMYMANAGER_H

class AddArmyManager
{
public:
    AddArmyManager(int baseTroops);

    void addTerritory();
    void removeTerritory();
    void addRegion();
    void removeRegion();
    void addCity();
    void removeCity();

    int calculateTotalTroops() const;

private:
    int baseTroops;
    int territoryCount;
    int regionCount;
    int cityCount;
    const int territoryBonus = 1;
    const int regionMultiplier = 2;
    const int cityBonus = 5;
};

#endif // ADDARMYMANAGER_H
