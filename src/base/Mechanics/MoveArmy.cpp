#include "MoveArmy.h"
#include "BattleArmiesWorker.h"
#include "MergeArmiesWorker.h"

MoveArmy::MoveArmy(Graph& graph) : m_graph(graph) {}

bool MoveArmy::executeAttack(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) {
    if (!validateAttack(sources, target, soldiersToMove)) {
        return false;
    }

    Army sentArmy(0, sources[0]->army.armyType());

    for (size_t i = 0; i < sources.size(); i++) {
        sources[i]->army.setSoldiers(sources[i]->army.getSoldiers() - soldiersToMove[i]);
        sentArmy.setSoldiers(sentArmy.getSoldiers() + soldiersToMove[i]);
    }

    unsigned sent = sentArmy.getSoldiers();
    BattleArmiesWorker* battleWorker = new BattleArmiesWorker(*this, sentArmy, *target, sources, soldiersToMove, sent);
    connect(battleWorker, &BattleArmiesWorker::battleFinished, this, &MoveArmy::onBattleFinished);
    battleWorker->start();

    return true;
}

bool MoveArmy::validateAttack(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) const {
    for (size_t i = 0; i < sources.size(); i++) {
        if (!areNeighbors(sources[i], target)) {
            std::cerr << "Error: Target vertex is not a neighbor of the source.\n";
            return false;
        }
        if (sources[i]->army.armyType() == target->army.armyType()) {
            std::cerr << "Error: Target vertex is of same type as the source.\n";
            return false;
        }
    }
    return true;
}

bool MoveArmy::executeMerge(Vertex* source, Vertex* target, unsigned soldiers) {
    if (source->army.armyType() != target->army.armyType()) {
        std::cerr << "Error: Target vertex is not of same type as the source.\n";
        return false;
    }
    MergeArmiesWorker* mergeWorker = new MergeArmiesWorker(*this, source, target, soldiers);
    QObject::connect(mergeWorker, &MergeArmiesWorker::mergeCompleted, this, &MoveArmy::onMergeCompleted);
    mergeWorker->start();
    return true;
}

bool MoveArmy::areNeighbors(const Vertex* source, const Vertex* target) const {
    auto neighbors = m_graph.neighbors(source);
    return std::find(neighbors.begin(), neighbors.end(), target) != neighbors.end();
}

void MoveArmy::onMergeCompleted(bool success) {
    if (success) {
        std::cout << "Armies successfully merged.\n";
    } else {
        std::cerr << "Error: Armies could not be merged.\n";
    }
}

void MoveArmy::onBattleFinished(bool success, Army sentArmy, std::vector<Vertex*> sources,
                                std::vector<unsigned> soldiersToMove, Vertex* target, unsigned sent) {
    if (success) {
        std::cout << "Battle completed successfully.\n";
        int left = sentArmy.getSoldiers();

        if (left != 0 && target->army.armyType() != sentArmy.armyType()) {
            for (size_t i = 0; i < sources.size(); i++) {
                unsigned retreated = (sentArmy.getSoldiers() * soldiersToMove[i]) / sent;
                sources[i]->army.setSoldiers(sources[i]->army.getSoldiers() + retreated);
                left -= retreated;

                if (left < 0) {
                    std::cerr << "Error: Not enough soldiers to retreat.\n";
                    return;
                }
            }
        }
        //TODO: refresh(LayerOf(target));

    } else {
        std::cerr << "Battle failed.\n";
    }
}

Graph& MoveArmy::getGraph() const {
    return m_graph;
}
