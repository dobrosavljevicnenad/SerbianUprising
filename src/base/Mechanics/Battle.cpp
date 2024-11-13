#include "Battle.h"

Battle::Battle(Army& defender, Army& attacker) : m_defender(defender), m_attacker(attacker) {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random number generation
}

void Battle::setTerrainAdvantage(int advantage1, int advantage2) {
    m_defenderAdvantage = advantage1;
    m_attackerAdvantage = advantage2;
}

int Battle::calculateHit(int strength, int terrainAdvantage, int soldiers) {
    // Calculate hit based on army's strength and terrain advantage
    int effectiveStrength = strength + terrainAdvantage;
    int hit = 0;
    for (int i = 1; i <= soldiers; i++) {
        hit += (std::rand() % 100 < effectiveStrength) ? 1 : 0;
    }
    return hit;
}

Army Battle::start() {
    std::cout << "Starting battle between "
              << (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " and "
              << (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary") << "!" << std::endl;

    std::cout << (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " soldiers: " << m_defender.getSoldiers() << std::endl;
    std::cout << (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " soldiers: " << m_attacker.getSoldiers() << std::endl;

    while (m_defender.getSoldiers() > 0 && m_attacker.getSoldiers() > 0) {
        std::time_t start_time = std::time(0);
        while (std::difftime(std::time(0), start_time) < 0.5) {
        }

        int hit1 = calculateHit(m_defender.strength(), m_defenderAdvantage, m_defender.getSoldiers());
        int hit2 = calculateHit(m_attacker.strength(), m_attackerAdvantage, m_attacker.getSoldiers());

        std::cout << (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " eliminated " << hit1 << " soldiers. "
                  << (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " eliminated " << hit2 << " soldiers." << std::endl;

        m_defender.setSoldiers(m_defender.getSoldiers() - hit2);
        m_attacker.setSoldiers(m_attacker.getSoldiers() - hit1);

        std::cout << (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " soldiers left: " << m_defender.getSoldiers() << std::endl;
        std::cout << (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                  << " soldiers left: " << m_attacker.getSoldiers() << std::endl;
        //soliders reatrithig
        if(m_attacker.getSoldiers() != 0 && (m_attacker.getSoldiers()<m_defender.getSoldiers())){
            if(std::rand() % 100 > 100*m_attacker.getSoldiers()/m_defender.getSoldiers()){
                std::cout <<"Attacker retreats. "<< (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " wins!" << std::endl;
                return m_defender;
            }
        }
        if(m_defender.getSoldiers() != 0 && (m_attacker.getSoldiers()>m_defender.getSoldiers())){
            if(std::rand() % 100 > 200*m_defender.getSoldiers()/m_attacker.getSoldiers()){
                std::cout <<"Defender retreats. "<< (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
                << " wins!" << std::endl;
                return m_attacker;
            }
        }
    }

    if (m_defender.getSoldiers() == 0) {
        std::cout << (m_attacker.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
        << " wins!" << std::endl;
        return m_attacker;
    }
    std::cout << (m_defender.armyType() == ArmyType::HAJDUK ? "Hajduk" : "Janissary")
              << " wins!" << std::endl;
    return m_defender;
}
