#include "BattleArmiesWorker.h"
#include "Battle.h"

BattleArmiesWorker::BattleArmiesWorker(MoveArmy& moveArmy, Army sentArmy, graph::Vertex& target,
                                       std::vector<graph::Vertex*> sources, std::vector<unsigned> soldiersToMove, unsigned sent)
    : m_moveArmy(moveArmy), m_sentArmy(sentArmy), m_target(&target),
    m_sources(sources), m_soldiersToMove(soldiersToMove), m_sent(sent) {}

void BattleArmiesWorker::run() {
    Battle battle(m_target->army, m_sentArmy);
    battle.setTerrainAdvantage(m_target->terrain.getDefenderAdvantage(), m_target->terrain.getAttackerAdvantage());

    std::cout << "Battle initiated between attacking army and defender at Vertex "
              << m_target->id() << ".\n";

    int defenderNumber = m_target->army.getSoldiers();
    int attackerNumber = m_sentArmy.getSoldiers();
    this->results = battle.start();
    results.setAttackerNumber(attackerNumber);
    results.setDefenderNumber(defenderNumber);

    results.setTargetVertexId(m_target->id());

    Army winner = *results.getWinner();

    handleBattleOutcome(winner);
}

void BattleArmiesWorker::handleBattleOutcome(Army& winner) {
    if (m_sentArmy.getSoldiers() == 0) {
        std::cout << "Attacking army defeated!\n";
        emit battleFinished(true, m_sentArmy, m_sources, m_soldiersToMove, m_target, m_sent, results);
        return;
    }

    if (m_target->army.getSoldiers() == 0) {
        std::cout << "Defender army defeated!\n";
        m_target->army = winner;
        emit battleFinished(true, m_sentArmy, m_sources, m_soldiersToMove, m_target, m_sent, results);
        return;
    }

    // Dodatna logika za upravljanje spajanjem nakon borbe
    if (winner.armyType() == m_sentArmy.armyType()) {
        updateNeighboringArmy();
        m_target->army = winner;
    }

    emit battleFinished(true, m_sentArmy, m_sources, m_soldiersToMove, m_target, m_sent, results);
}

void BattleArmiesWorker::updateNeighboringArmy() {
    auto neighbors = m_moveArmy.getGraph().neighbors(m_target);
    for (auto& neighbor : neighbors) {
        if (neighbor->army.armyType() == m_target->army.armyType()) {
            neighbor->army.setSoldiers(neighbor->army.getSoldiers() + m_target->army.getSoldiers());
            break;
        }
    }
}
