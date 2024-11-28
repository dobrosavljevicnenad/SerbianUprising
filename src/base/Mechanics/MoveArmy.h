// MoveArmy.h
#pragma once
#include <vector>

#include "../graph/vertex.hpp"
#include "../graph/graph.hpp"
#include "Battle.h"
#include "../Entities/Army.h"
using namespace graph;
class MoveArmy {
public:
    MoveArmy(Graph& graph);
    bool executeMove(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned>soldiersToMove);
    bool mergeArmies(Vertex* source, Vertex* target, unsigned int soldiersToMove);

private:
    Graph& m_graph;

    bool areNeighbors(const Vertex* source, const Vertex* target) const;
    void battleArmies(Army& source, Vertex* target);
};

