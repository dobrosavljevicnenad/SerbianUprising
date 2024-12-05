#include "Army.h"

Army::Army(ArmyType armyType) : m_soldiers(0), m_armyType(armyType) {}

Army::Army(int soldiers, ArmyType armyType)
    : m_soldiers(soldiers), m_armyType(armyType) {}

Army::~Army() {}

int Army::getSoldiers() const {
    return m_soldiers;
}

void Army::setSoldiers(int soldiers) {
    m_soldiers = soldiers < 0 ? 0 : soldiers;
}

ArmyType Army::armyType() const {
    return m_armyType;
}

int Army::morale() const {
    return Strength::instance().getBoost(m_armyType);
}

int Army::strength() const {
    return Strength::baseStrength() + morale();
}

std::string Army::to_string(ArmyType type) const {
    switch(type){
    case ArmyType::HAJDUK:
        return "HAJDUK";
        break;
    case ArmyType::JANISSARY:
        return "JANNISARY";
        break;
    default:
        return "";
    }
}
