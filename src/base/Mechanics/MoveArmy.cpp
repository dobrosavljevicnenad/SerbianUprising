#include "MoveArmy.h"
#include <iostream>

MoveArmy::MoveArmy(Graph& graph) : m_graph(graph) {}

bool MoveArmy::executeMove(Vertex* source, Vertex* target, unsigned int soldiersToMove) {
    if (!areNeighbors(source, target)) {
        std::cerr << "Error: Target vertex is not a neighbor of the source.\n";
        return false;
    }

    if (soldiersToMove > source->army.getSoldiers()) {
        std::cerr << "Error: Insufficient soldiers in the source army.\n";
        return false;
    }

    if (source->army.armyType() == target->army.armyType()) {
        mergeArmies(source, target, soldiersToMove);
    } else {
        source->army.setSoldiers(source->army.getSoldiers() - soldiersToMove);
        Army sentArmy(soldiersToMove, source->army.armyType());

        battleArmies(sentArmy, target);
        if(sentArmy.getSoldiers() != 0 && target->army.armyType() != sentArmy.armyType())
            source->army.setSoldiers(source->army.getSoldiers() + sentArmy.getSoldiers());
    }

    return true;
}

bool MoveArmy::areNeighbors(const Vertex* source, const Vertex* target) const {
    auto neighbors = m_graph.neighbors(source);
    return std::find(neighbors.begin(), neighbors.end(), target) != neighbors.end();
}

void MoveArmy::mergeArmies(Vertex* source, Vertex* target, unsigned int soldiersToMove) {
    source->army.setSoldiers(source->army.getSoldiers() - soldiersToMove);
    target->army.setSoldiers(target->army.getSoldiers() + soldiersToMove);

    std::cout << "Armies merged: " << soldiersToMove << " soldiers moved from Vertex "
              << source->id() << " to Vertex " << target->id() << ".\n";
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
