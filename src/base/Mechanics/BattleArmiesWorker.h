#ifndef BATTLEARMIESWORKER_H
#define BATTLEARMIESWORKER_H

#include <QThread>
#include "../graph/vertex.hpp"
#include "../Entities/Army.h"
#include "MoveArmy.h"
#include "Battle.h"

class BattleArmiesWorker : public QThread {
    Q_OBJECT
public:
    BattleArmiesWorker(MoveArmy& moveArmy, Army sentArmy, graph::Vertex& target,
                       std::vector<graph::Vertex*> sources, std::vector<unsigned> soldiersToMove, unsigned sent);
    Results results;

signals:
    void battleFinished(bool success, Army sentArmy, std::vector<graph::Vertex*> sources,
                        std::vector<unsigned> soldiersToMove, graph::Vertex* target, unsigned sent, Results results);

protected:
    void run() override;

private:
    void handleBattleOutcome(Army& winner);
    void updateNeighboringArmy();

    MoveArmy& m_moveArmy;
    Army m_sentArmy;
    graph::Vertex* m_target;
    std::vector<graph::Vertex*> m_sources;
    std::vector<unsigned> m_soldiersToMove;
    unsigned m_sent;
};

#endif // BATTLEARMIESWORKER_H
