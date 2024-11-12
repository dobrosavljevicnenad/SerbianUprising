    #ifndef ARMY_H
#define ARMY_H

#include "Strength.h"
#include <string>

enum class ArmyType : int {
    HAJDUK,
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
    std::string to_string(ArmyType type) const;

private:
    int m_soldiers;
    ArmyType m_armyType;
};

#endif // ARMY_H
