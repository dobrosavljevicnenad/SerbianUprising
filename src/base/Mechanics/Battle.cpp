#include "Battle.h"
#include <iostream>
#include <ctime>

Battle::Battle(Army& defender, Army& attacker)
    : m_defender(defender), m_attacker(attacker)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
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

Results Battle::start() {
    Results results;

    ArmyType defenderType = m_defender.armyType();
    ArmyType attackerType = m_attacker.armyType();

    std::cout << "Starting battle between "
              << (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " and "
              << (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << "!" << std::endl;

    printArmyStatus(m_defender, (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary"));
    printArmyStatus(m_attacker, (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary"));

    results.setArmyTypes(defenderType, attackerType);

    while (m_defender.getSoldiers() > 0 && m_attacker.getSoldiers() > 0) {
        std::time_t start_time = std::time(0);
        //while (std::difftime(std::time(0), start_time) < 0.5) {}

        int defenderHits = calculateHit(m_defender.strength(), m_defenderAdvantage, m_defender.getSoldiers());
        int attackerHits = calculateHit(m_attacker.strength(), m_attackerAdvantage, m_attacker.getSoldiers());

        std::cout << (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " eliminated " << defenderHits << " soldiers. "
                  << (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " eliminated " << attackerHits << " soldiers." << std::endl;

        m_defender.setSoldiers(m_defender.getSoldiers() - attackerHits);
        m_attacker.setSoldiers(m_attacker.getSoldiers() - defenderHits);

        printArmyStatus(m_defender, (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary"));
        printArmyStatus(m_attacker, (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary"));

        results.recordRound(defenderHits, attackerHits, m_defender.getSoldiers(), m_attacker.getSoldiers());

        if (m_attacker.getSoldiers() != 0 && m_defender.getSoldiers() != 0) {
            if (shouldRetreat(m_attacker.getSoldiers(), m_defender.getSoldiers(), true)) {
                std::cout << (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " retreats. " << (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " wins!" << std::endl;
                results.setWinner(&m_defender);
                return results;
            }
            if (shouldRetreat(m_defender.getSoldiers(), m_attacker.getSoldiers(), false)) {
                std::cout << (defenderType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " retreats. " << (attackerType == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " wins!" << std::endl;
                results.setWinner(&m_attacker);
                return results;
            }
        }
    }

    Army* winner = m_defender.getSoldiers() == 0 ? &m_attacker : &m_defender;
    results.setWinner(winner);

    return results;

}
