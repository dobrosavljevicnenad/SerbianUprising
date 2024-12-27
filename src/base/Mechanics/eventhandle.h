#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

#include "event.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QRandomGenerator>

class EventHandle {
public:
    EventHandle();

    void deserializeEvents(const QJsonArray& eventsJson,unsigned clientId);


    const QVector<QPair<Event, bool>>& getEvents() const;

    QVector<Event> getPendingEvents() const;

    void addEvent(const Event &event);
    void markEventOccurred(unsigned int eventId);
    void processEvents(const QString& currentYear, const graph::Graph& clientGraph);

private:
    unsigned clientId;
    QVector<QPair<Event, bool>> events;
    bool shouldSpawnEvent(int probability) const;

    void processRandomEvent(const Event &event);
    void processPlaceTrigger(const Event &event, const graph::Graph &clientGraph);
    void processAttackTrigger(const Event &event, const graph::Graph &clientGraph);
    void processMoraleTrigger(const Event &event);
    void processRecruitmentsTrigger(const Event &event, const graph::Graph &clientGraph);
    void processNavalTrigger(const Event &event);
    void processIntroTrigger(const Event &event);
    void processEndTrigger(const Event &event);
};

#endif // EVENTHANDLE_H
