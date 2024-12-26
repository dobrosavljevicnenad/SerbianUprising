#include "strength.h"

Strength& Strength::instance() {
    static Strength instance;
    return instance;
}

void Strength::setBoost(ArmyType type, int boost) {
    if (type == ArmyType::HAJDUK) {
        m_boostHajduk += boost;
        if (m_boostHajduk < 0)
            m_boostHajduk = 0;
        if (m_boostHajduk > 20)
            m_boostHajduk = 20;

    } else if (type == ArmyType::JANISSARY) {
        m_boostJanissary += boost;
        if (m_boostJanissary < 0)
            m_boostJanissary = 0;
        if (m_boostJanissary > 20)
            m_boostJanissary = 20;
    }
}
int Strength::getBoost(ArmyType type) const {
    return (type == ArmyType::HAJDUK) ? m_boostHajduk : m_boostJanissary;
}

int Strength::baseMorale() {
    return s_baseMorale;
}

int Strength::baseStrength() {
    return s_baseStrength;
}
