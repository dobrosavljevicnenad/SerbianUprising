#include "MoveArmy.h"
#include "BattleArmiesWorker.h"
#include "MergeArmiesWorker.h"
#include <algorithm>

MoveArmy::MoveArmy(Graph& graph) : m_graph(graph), player1Attacks({}) {}

bool MoveArmy::executeAttack(int playerId, std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) {
    while (playerId == 2 && !player1Attacks.empty()) {
        QCoreApplication::processEvents();
    }
    while (std::find(contested.begin(), contested.end(), target->id()) != contested.end()) {
        QCoreApplication::processEvents();
    }
    if (!validateAttack(sources, target, soldiersToMove)) {
        emit battleCanceled();
        return false;
    }
    Army sentArmy(0, sources[0]->army.armyType());

    for (size_t i = 0; i < sources.size(); i++) {
        soldiersToMove[i] = std::min(static_cast<unsigned int>(sources[i]->army.getSoldiers()), soldiersToMove[i]);
        sources[i]->army.setSoldiers(sources[i]->army.getSoldiers() - soldiersToMove[i]);
        sentArmy.setSoldiers(sentArmy.getSoldiers() + soldiersToMove[i]);
    }

    contested.push_back(target->id());
    for (int var = 0; var < sources.size(); var++) {
        contested.push_back(sources[var]->id());
    }
    if(playerId == 1)
        player1Attacks.push_back(1);
    unsigned sent = sentArmy.getSoldiers();
    int riverAdvantage = 0;
    for (auto& it : m_graph.adj_list()[target]) {
        if (it.type() == graph::EdgeType::River &&
            std::find(sources.begin(), sources.end(), m_graph.get_vertex_by_id(it.to())) != sources.end()) {
            riverAdvantage = 5;
        }
    }

    qDebug() << "River: "<< riverAdvantage;

    BattleArmiesWorker* battleWorker = new BattleArmiesWorker(*this, playerId, sentArmy, *target, sources, soldiersToMove, sent, riverAdvantage);
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

void MoveArmy::onBattleFinished(int playerId, bool success, Army sentArmy, std::vector<Vertex*> sources,
                                std::vector<unsigned> soldiersToMove, Vertex* target, unsigned sent, Results results) {
    emit battleFinished(results);
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
                    //return;
                }
            }
        }
        //TODO: refresh(LayerOf(target));
    } else {
        std::cerr << "Battle failed.\n";
    }
    auto targetId = target->id();
    contested.erase(std::remove(contested.begin(), contested.end(), targetId), contested.end());

    for (int var = 0; var < sources.size(); var++) {
        auto sourceId = sources[var]->id();
        contested.erase(std::remove(contested.begin(), contested.end(), sourceId), contested.end());
    }
    if(playerId == 1 && !player1Attacks.empty())
        player1Attacks.pop_back();
}

Graph& MoveArmy::getGraph() const {
    return m_graph;
}
