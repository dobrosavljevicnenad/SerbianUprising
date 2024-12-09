#pragma once
#include <vector>
#include <QThread>
#include <iostream>
#include <QObject>
#include "../graph/vertex.hpp"
#include "../graph/graph.hpp"
#include "Battle.h"
#include "../Entities/Army.h"

using namespace graph;

class MoveArmy : public QThread {
    Q_OBJECT

public:
    explicit MoveArmy(Graph& graph);

    bool executeAttack(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove);
    bool executeMerge(Vertex* source, Vertex* target, unsigned soldiers);
    Graph& getGraph() const;

    bool areNeighbors(const Vertex* source, const Vertex* target) const;
    bool validateAttack(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) const;

public slots:
    void onMergeCompleted(bool success);
    void onBattleFinished(bool success, Army sentArmy, std::vector<Vertex*> sources,
                          std::vector<unsigned> soldiersToMove, Vertex* target, unsigned sent);

private:
    Graph& m_graph;
};
