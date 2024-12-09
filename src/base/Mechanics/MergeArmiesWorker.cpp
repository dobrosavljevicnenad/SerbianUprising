#include "MergeArmiesWorker.h"

MergeArmiesWorker::MergeArmiesWorker(MoveArmy& moveArmy, Vertex* source, Vertex* target, int soldiersToMove)
    : m_moveArmy(moveArmy), m_source(source), m_target(target), m_soldiersToMove(soldiersToMove) {}

void MergeArmiesWorker::run() {
    if (!m_moveArmy.areNeighbors(m_source, m_target)) {
        std::cerr << "Error: Target vertex is not a neighbor of the source.\n";
        emit mergeCompleted(false);
        return;
    }

    if (m_source->army.armyType() == m_target->army.armyType()) {
        std::cout << "Merging: " << m_soldiersToMove << " from vertex with "
                  << m_source->army.getSoldiers() << " to Vertex " << m_target->army.getSoldiers() << ".\n";
        m_source->army.setSoldiers(m_source->army.getSoldiers() - m_soldiersToMove);
        m_target->army.setSoldiers(m_target->army.getSoldiers() + m_soldiersToMove);
        std::cout << "Armies merged: " << m_soldiersToMove << " soldiers moved from Vertex "
                  << m_source->id() << " to Vertex " << m_target->id() << ".\n";
        emit mergeCompleted(true);
    } else {
        std::cerr << "Error: Armies of different types cannot be merged.\n";
        emit mergeCompleted(false);
    }
}
