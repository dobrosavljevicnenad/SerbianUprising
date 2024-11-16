#include "MoveArmy.h"

MoveArmy::MoveArmy(Graph& graph, Turn& turn) : m_graph(graph), m_turn(turn) {}

bool MoveArmy::executeMove(Vertex* source, Vertex* target, unsigned int soldiersToMove) {
    if (!areNeighbors(source, target)) {
        m_turn.addActionToBuffer(m_turn.getCurrentPlayer(), "Invalid Move: Target is not a neighbor.");
        std::cout << "Invalid Move: Target is not a neighbor." << std::endl;
        return false;
    }

    if (soldiersToMove > source->army.getSoldiers()) {
        m_turn.addActionToBuffer(m_turn.getCurrentPlayer(), "Invalid Move: Not enough soldiers.");
        std::cout << "Invalid Move: Not enough soldiers." << std::endl;
        return false;
    }

    std::string action = "";
    if (source->army.armyType() == target->army.armyType()) {
        action = "Move " + std::to_string(soldiersToMove) + " soldiers from Vertex " +
                 std::to_string(source->id()) + " to Vertex " + std::to_string(target->id());
    } else {
        action = "Attack " + std::to_string(soldiersToMove) + " soldiers from Vertex " +
                 std::to_string(source->id()) + " to Vertex " + std::to_string(target->id());
    }
    m_turn.addActionToBuffer(m_turn.getCurrentPlayer(), action);

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

void MoveArmy::battleArmies(Army& source, Vertex*target) {
    Battle battle(target->army, source);
    battle.setTerrainAdvantage(target->terrain.getDefenderAdvantage(), target->terrain.getAttackerAdvantage());

    std::cout << "Battle initiated between sent army "
              << " and Vertex " << target->id() << ".\n";

    Army winner = battle.start();

    if (source.getSoldiers() == 0) {
        std::cout << "Source army defeated!\n";
        return;
    }
    if (target->army.getSoldiers() == 0) {
        std::cout << "Target army defeated!\n";
        target->army = winner;
        return;
    }
    if(winner.armyType() == source.armyType()){
        auto neighbors = m_graph.neighbors(target);
        for(auto& n : neighbors)
            if(n->army.armyType() == target->army.armyType()){
                n->army.setSoldiers(n->army.getSoldiers() + target->army.getSoldiers());
                break;
            }
        target->army = winner;
        return;
    }
    else{
        auto neighbors = m_graph.neighbors(target);
        for(auto& n : neighbors)
            if(n->army.armyType() == source.armyType()){
                n->army.setSoldiers(n->army.getSoldiers() + source.getSoldiers());
                break;
            }
        target->army = winner;
        return;
    }
}
