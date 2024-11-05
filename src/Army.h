#ifndef ARMY_H
#define ARMY_H

#include "Strength.h"

enum class ArmyType : int {
    PATRIOT,
    JANISSARY
};

class Army
{
public:
    explicit Army(ArmyType armyType);
    Army(int soldiers, ArmyType armyType);

    ~Army();

    int getSoldiers() const;
    void setSoldiers(int soldiers);

    ArmyType armyType() const;
    int strength() const;
    int morale() const;

private:
    int m_soldiers;
    ArmyType m_armyType;
};

#endif // ARMY_H
