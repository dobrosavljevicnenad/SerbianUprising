#pragma once

#ifndef ARMY_H
#define ARMY_H

#include "../Mechanics/strength.h"

#include <string>
#include <QColor>

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

    static Army fromString(const std::string& type, int soldiers) {
        ArmyType armyType = (type == "HAJDUK") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        return Army(soldiers, armyType);
    }

    static ArmyType fromString(const std::string& type) {
        ArmyType armyType = (type == "HAJDUK") ? ArmyType::HAJDUK : ArmyType::JANISSARY;
        return armyType;
    }

private:
    int m_soldiers;
    ArmyType m_armyType;
};

#endif // ARMY_H
