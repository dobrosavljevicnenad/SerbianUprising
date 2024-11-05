#include "Battle.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Battle::Battle(Army& army1, Army& army2) : m_army1(army1), m_army2(army2) {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random number generation
}

void Battle::setTerrainAdvantage(int advantage1, int advantage2) {
    m_terrainAdvantage1 = advantage1;
    m_terrainAdvantage2 = advantage2;
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

void Battle::start() {
    std::cout << "Starting battle between "
              << (m_army1.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
              << " and "
              << (m_army2.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary") << "!" << std::endl;

    std::cout << (m_army1.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
              << " soldiers: " << m_army1.getSoldiers() << std::endl;
    std::cout << (m_army2.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
              << " soldiers: " << m_army2.getSoldiers() << std::endl;

    while (m_army1.getSoldiers() > 0 && m_army2.getSoldiers() > 0) {
        std::time_t start_time = std::time(0);
        while (std::difftime(std::time(0), start_time) < 0.5) {
        }

        int hit1 = calculateHit(m_army1.strength(), m_terrainAdvantage1, m_army1.getSoldiers());
        int hit2 = calculateHit(m_army2.strength(), m_terrainAdvantage2, m_army2.getSoldiers());

        std::cout << (m_army1.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
                  << " eliminated " << hit1 << " soldiers. "
                  << (m_army2.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
                  << " eliminated " << hit2 << " soldiers." << std::endl;

        m_army1.setSoldiers(m_army1.getSoldiers() - hit2);
        m_army2.setSoldiers(m_army2.getSoldiers() - hit1);

        std::cout << (m_army1.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
                  << " soldiers left: " << m_army1.getSoldiers() << std::endl;
        std::cout << (m_army2.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
                  << " soldiers left: " << m_army2.getSoldiers() << std::endl;

    }

    if (m_army1.getSoldiers() <= 0) {
        std::cout << (m_army2.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
        << " wins!" << std::endl;
    } else {
        std::cout << (m_army1.armyType() == ArmyType::PATRIOT ? "Patriot" : "Janissary")
        << " wins!" << std::endl;
    }
}
