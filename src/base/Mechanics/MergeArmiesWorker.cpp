// MergeArmiesWorker.cpp

#include "MergeArmiesWorker.h"
#include "MoveArmy.h"

MergeArmiesWorker::MergeArmiesWorker(MoveArmy& moveArmy, Vertex* source, Vertex* target, int soldiersToMove)
    : m_moveArmy(moveArmy), m_source(source), m_target(target), m_soldiersToMove(soldiersToMove) {}

void MergeArmiesWorker::run() {
    bool success = m_moveArmy.mergeArmies(m_source, m_target, m_soldiersToMove);
    emit mergeCompleted(success);
}
