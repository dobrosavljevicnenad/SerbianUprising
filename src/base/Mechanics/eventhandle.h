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

    void deserializeEvents(const QJsonArray& eventsJson);


    const QVector<QPair<Event, bool>>& getEvents() const;

    QVector<Event> getPendingEvents() const;

    void addEvent(const Event &event);
    void markEventOccurred(unsigned int eventId);
    void processEvents(int clientId, const QString& currentYear, const graph::Graph& clientGraph);

private:
    unsigned clientId;
    QVector<QPair<Event, bool>> events;
};

#endif // EVENTHANDLE_H
