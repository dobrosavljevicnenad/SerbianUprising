// MoveArmy.h
#pragma once

#include "../graph/vertex.hpp"
#include "../graph/graph.hpp"
#include "Battle.h"
#include "Army.h"
using namespace graph;
class MoveArmy {
public:
    MoveArmy(Graph& graph);
    bool executeMove(Vertex* source, Vertex* target, unsigned int soldiersToMove);

private:
    Graph& m_graph;

    bool areNeighbors(const Vertex* source, const Vertex* target) const;
    void mergeArmies(Vertex* source, Vertex* target, unsigned int soldiersToMove);
    void battleArmies(Army& source, Vertex* target);
};

