#pragma once
#include <vector>
#include <QThread>
#include <iostream>
#include <QObject>
#include "../graph/vertex.hpp"
#include "../graph/graph.hpp"
#include "Battle.h"
#include "../Entities/Army.h"
#include <QCoreApplication>
using namespace graph;

class MoveArmy : public QThread {
    Q_OBJECT

public:
    explicit MoveArmy(Graph& graph);

    bool executeAttack(int playerId, std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove);
    bool executeMerge(Vertex* source, Vertex* target, unsigned soldiers);
    Graph& getGraph() const;

    bool areNeighbors(const Vertex* source, const Vertex* target) const;
    bool validateAttack(std::vector<Vertex*> sources, Vertex* target, std::vector<unsigned> soldiersToMove) const;
signals:
    Results battleFinished(Results result);
    void battleCanceled();

public slots:
    void onMergeCompleted(bool success);
    void onBattleFinished(int playerId, bool success, Army sentArmy, std::vector<Vertex*> sources,
                          std::vector<unsigned> soldiersToMove, Vertex* target, unsigned sent, Results results);

private:
    Graph& m_graph;
    std::vector<int> contested;
    std::vector<bool> player1Attacks;
    void checkPlayer1Attack(int playerId, std::vector<Vertex *> sources, Vertex *target, std::vector<unsigned int> soldiersToMove);
};
