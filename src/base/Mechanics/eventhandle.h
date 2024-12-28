#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H

#include "addarmymanager.h"
#include "event.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QRandomGenerator>
#include "turn.h"
class EventHandle {
public:
    EventHandle();

    void deserializeEvents(const QJsonArray& eventsJson,unsigned clientId);


    const QVector<QPair<Event, bool>>& getEvents() const;

    QVector<Event> getPendingEvents() const;

    void addEvent(const Event &event);
    void markEventOccurred(unsigned int eventId);
    void processEvents(const QString& currentYear, const graph::Graph& clientGraph, Turn *turn);

    QJsonObject serializeProcessedEvents();
    void processSpecificEvent(int clientId, const QString &title,AddArmyManager& armyManager, bool& naval);
    void processIntroEvents();
private:
    unsigned clientId;
    QVector<QPair<Event, bool>> events;
    QVector<Event> processedEventsBuffer;

    bool shouldSpawnEvent(int probability) const;

    void processRandomEvent(const Event &event);
    void processPlaceTrigger(const Event &event, const graph::Graph &clientGraph, Turn *turn);
    void processAttackTrigger(const Event &event, const graph::Graph &clientGraph, Turn *turn);
    void processMoraleTrigger(const Event &event);
    void processRecruitmentsTrigger(const Event &event, AddArmyManager* armyManager);
    void processNavalTrigger(bool& naval, const Event& event);
    void processIntroTrigger(const Event &event);
    void processEndTrigger(const Event &event);
};

#endif // EVENTHANDLE_H
