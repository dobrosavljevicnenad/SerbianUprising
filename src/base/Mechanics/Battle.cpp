#include "Battle.h"

Battle::Battle(Army& defender, Army& attacker)
    : m_defender(defender), m_attacker(attacker)
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random number generation
}

void Battle::setTerrainAdvantage(int defenderAdvantage, int attackerAdvantage) {
    m_defenderAdvantage = defenderAdvantage;
    m_attackerAdvantage = attackerAdvantage;
}

int Battle::calculateHit(int strength, int terrainAdvantage, int soldiers) {
    int effectiveStrength = strength + terrainAdvantage;
    int hit = 0;
    for (int i = 0; i < soldiers; ++i) {
        hit += (std::rand() % 100 < effectiveStrength) ? 1 : 0;
    }
    return hit;
}

void Battle::printArmyStatus(const Army& army, const std::string& armyName) const {
    std::cout << armyName << " soldiers left: " << army.getSoldiers() << std::endl;
}

bool Battle::shouldRetreat(int attackerSoldiers, int defenderSoldiers, bool isAttacker) const {
    float soldierRatio = static_cast<float>(attackerSoldiers) / defenderSoldiers;
    int retreatChance = (isAttacker) ? 100 * soldierRatio : 200 * soldierRatio;

    return (std::rand() % 100) > retreatChance;
}

Army Battle::start() {
    std::string defenderName = (m_defender.armyType() == ArmyType::HAJDUK) ? "Hajduk" : "Janissary";
    std::string attackerName = (m_attacker.armyType() == ArmyType::HAJDUK) ? "Hajduk" : "Janissary";

    std::cout << "Starting battle between " << defenderName << " and " << attackerName << "!" << std::endl;

    printArmyStatus(m_defender, defenderName);
    printArmyStatus(m_attacker, attackerName);

    while (m_defender.getSoldiers() > 0 && m_attacker.getSoldiers() > 0) {
        std::time_t start_time = std::time(0);
        while (std::difftime(std::time(0), start_time) < 0.5) {
        }

        int defenderHits = calculateHit(m_defender.strength(), m_defenderAdvantage, m_defender.getSoldiers());
        int attackerHits = calculateHit(m_attacker.strength(), m_attackerAdvantage, m_attacker.getSoldiers());

        std::cout << defenderName << " eliminated " << defenderHits << " soldiers. "
                  << attackerName << " eliminated " << attackerHits << " soldiers." << std::endl;

        m_defender.setSoldiers(m_defender.getSoldiers() - attackerHits);
        m_attacker.setSoldiers(m_attacker.getSoldiers() - defenderHits);

        printArmyStatus(m_defender, defenderName);
        printArmyStatus(m_attacker, attackerName);
        if(m_attacker.getSoldiers() != 0 && m_defender.getSoldiers() != 0){
            if (shouldRetreat(m_attacker.getSoldiers(), m_defender.getSoldiers(), true)) {
                std::cout << attackerName << " retreats. " << defenderName << " wins!" << std::endl;
                return m_defender;
            }
            if (shouldRetreat(m_defender.getSoldiers(), m_attacker.getSoldiers(), false)) {
                std::cout << defenderName << " retreats. " << attackerName << " wins!" << std::endl;
                return m_attacker;
            }
        }
    }

    if (m_defender.getSoldiers() == 0) {
        std::cout << attackerName << " wins!" << std::endl;
        return m_attacker;
    }

    std::cout << defenderName << " wins!" << std::endl;
    return m_defender;
}
