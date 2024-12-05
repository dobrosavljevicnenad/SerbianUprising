// MoveArmy.h
#pragma once
#include <vector>
#include <QThread>
#include <iostream>
#include <QObject>
#include "../graph/vertex.hpp"
#include "../graph/graph.hpp"
#include "Battle.h"
#include "../Entities/Army.h"
#include "BattleArmiesWorker.h"

using namespace graph;
class MoveArmy : public QThread{
    Q_OBJECT
public:
    MoveArmy(Graph& graph);
    bool executeMove(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned>soldiersToMove);
    bool mergeArmies(Vertex* source, Vertex* target, unsigned int soldiersToMove);
    void battleArmies(Army& source, Vertex* target);
public slots:
    void onMergeCompleted(bool success);
    void onBattleFinished(bool success, Army sentArmy, std::vector<Vertex*> sources,
                                    std::vector<unsigned> soldiersToMove, Vertex* target, unsigned sent);

private:
    Graph& m_graph;

    bool areNeighbors(const Vertex* source, const Vertex* target) const;
};

