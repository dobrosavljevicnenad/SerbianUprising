#include "MoveArmy.h"

MoveArmy::MoveArmy(Graph& graph) : m_graph(graph) {}

bool MoveArmy::executeMove(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) {
    Army sentArmy(0, sources[0]->army.armyType());

    for (int i = 0; i < sources.size(); i++) {
        if (!areNeighbors(sources[i], target)) {
            std::cerr << "Error: Target vertex is not a neighbor of the source.\n";
            return false;
        }
        if (sources[i]->army.armyType() == target->army.armyType()) {
            std::cerr << "Error: Target vertex is of same type as the source.\n";
            return false;
        }
        if (sources[i]->army.armyType() != target->army.armyType()) {
            sentArmy.setSoldiers(sentArmy.getSoldiers() + soldiersToMove[i]);
        }
    }
    unsigned sent = sentArmy.getSoldiers();
    battleArmies(sentArmy, target);
    int left = sentArmy.getSoldiers();
    if(sentArmy.getSoldiers() != 0 && target->army.armyType() != sentArmy.armyType())
    {

        for (int i = 0; i < sources.size(); i++) {
            if(i != (sources.size() - 1)){
                unsigned retreated = (sentArmy.getSoldiers()* soldiersToMove[i] ) / sent;
                sources[i]->army.setSoldiers(sources[i]->army.getSoldiers() + retreated);
                left -= retreated;
                if(left < 0){
                    std::cerr << "Error: Note enought soldiers to retreat.\n";
                    return false;
                }
            }
            else {
                sources[i]->army.setSoldiers(sources[i]->army.getSoldiers() + left);
                left = 0;
            }
        }
    }
    return true;
}

bool MoveArmy::areNeighbors(const Vertex* source, const Vertex* target) const {
    auto neighbors = m_graph.neighbors(source);
    return std::find(neighbors.begin(), neighbors.end(), target) != neighbors.end();
}

bool MoveArmy::mergeArmies(Vertex* source, Vertex* target, unsigned int soldiersToMove) {
    if (!areNeighbors(source, target)) {
        std::cerr << "Error: Target vertex is not a neighbor of the source.\n";
        return false;
    }
    if (source->army.armyType() == target->army.armyType()) {
        std::cout << "Mergingd: " << soldiersToMove << " from vertex with "
                  << source->army.getSoldiers() << " to Vertex " << target->army.getSoldiers() << ".\n";
        target->army.setSoldiers(target->army.getSoldiers() + soldiersToMove);
        std::cout << "Armies merged: " << soldiersToMove << " soldiers moved from Vertex "
                  << source->id() << " to Vertex " << target->id() << ".\n";
        return true;
    }
    return false;
}

void MoveArmy::battleArmies(Army& source, Vertex* target) {
    Battle battle(target->army, source);
    battle.setTerrainAdvantage(target->terrain.getDefenderAdvantage(), target->terrain.getAttackerAdvantage());

    std::cout << "Battle initiated between attacking army and defender at Vertex "
              << target->id() << ".\n";

    Army winner = battle.start();

    if (source.getSoldiers() == 0) {
        std::cout << "Attacking army defeated!\n";
        return;
    }
    if (target->army.getSoldiers() == 0) {
        std::cout << "Defender army defeated!\n";
        target->army = winner;
        return;
    }

    // Additional logic to manage army merging after battle
    if (winner.armyType() == source.armyType()) {
        auto neighbors = m_graph.neighbors(target);
        for (auto& n : neighbors) {
            if (n->army.armyType() == target->army.armyType()) {
                n->army.setSoldiers(n->army.getSoldiers() + target->army.getSoldiers());
                break;
            }
        }
        target->army = winner;
        return;
    } /*else {
        auto neighbors = m_graph.neighbors(target);
        for (auto& n : neighbors) {
            if (n->army.armyType() == source.armyType()) {
                n->army.setSoldiers(n->army.getSoldiers() + source.getSoldiers());
                break;
            }
        }
        target->army = winner;
        return;
    }*/
}
