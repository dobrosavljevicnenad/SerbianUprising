#ifndef MERGEARMIESWORKER_H
#define MERGEARMIESWORKER_H

#include <iostream>
#include <QThread>
#include "../Entities/army.h"
#include "../graph/vertex.hpp"
#include "movearmy.h"

class MergeArmiesWorker : public QThread {
    Q_OBJECT
public:
    MergeArmiesWorker(MoveArmy& moveArmy, Vertex* source, Vertex* target, int soldiersToMove);

signals:
    void mergeCompleted(bool success);

protected:
    void run() override;

private:
    MoveArmy& m_moveArmy;
    Vertex* m_source;
    Vertex* m_target;
    int m_soldiersToMove;
};

#endif // MERGEARMIESWORKER_H
