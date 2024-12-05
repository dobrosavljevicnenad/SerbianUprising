#include "BattleArmiesWorker.h"
#include "MoveArmy.h"

#include "BattleArmiesWorker.h"

#include "BattleArmiesWorker.h"

BattleArmiesWorker::BattleArmiesWorker(MoveArmy& moveArmy, Army sentArmy, graph::Vertex& target,
                                       std::vector<graph::Vertex*> sources, std::vector<unsigned> soldiersToMove, unsigned sent)
    : m_moveArmy(moveArmy), m_sentArmy(sentArmy), m_target(&target),
    m_sources(sources), m_soldiersToMove(soldiersToMove), m_sent(sent) {}

void BattleArmiesWorker::run() {
    m_moveArmy.battleArmies(m_sentArmy, m_target);
    bool success = true;

    emit battleFinished(success, m_sentArmy, m_sources, m_soldiersToMove, m_target, m_sent);
}
