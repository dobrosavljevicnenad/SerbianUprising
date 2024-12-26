#include "eventhandle.h"

EventHandle::EventHandle() {}

void EventHandle::deserializeEvents(const QJsonArray& eventsJson) {
    unsigned int idCounter = 0;
    for (const QJsonValue& value : eventsJson) {
        if (value.isObject()) {
            QJsonObject eventJson = value.toObject();

            EventType type = Event::stringToEventType(eventJson["type"].toString().toStdString());
            QString title = eventJson["title"].toString();
            QString imagePath = eventJson["image_path"].toString();
            QString description = eventJson["description"].toString();
            QString buttonText = eventJson["button"].toString();
            QVector<QString> territoryTrigger;
            QString trigger = eventJson["trigger"].toString();
            int triggerAmount = eventJson["trigger_amount"].toInt();
            QVector<QString> territoryAffect;
            QString date = eventJson["date"].toString();

            Event event(idCounter++, type, title, imagePath, description, buttonText,
                        territoryTrigger, trigger, triggerAmount, territoryAffect, date);

            events.append(QPair<Event, bool>(event, false));
        }
    }
}

void EventHandle::addEvent(const Event& event) {
    events.append(QPair<Event, bool>(event, false));
}

void EventHandle::markEventOccurred(unsigned int eventId) {
    for (auto& eventPair : events) {
        if (eventPair.first.id == eventId) {
            eventPair.second = true;
            return;
        }
    }
}

const QVector<QPair<Event, bool>>& EventHandle::getEvents() const {
    return events;
}

void EventHandle::processEvents(int clientId, const QString& currentYear, const graph::Graph& clientGraph) {
    QVector<Event> triggerableEvents;
    for (const auto& eventPair : events) {
        const Event& event = eventPair.first;
        if (!eventPair.second && event.canTrigger(clientId, currentYear, clientGraph)) {
            triggerableEvents.append(event);
        }
    }

    QVector<Event> randomEvents;
    for (const auto& event : triggerableEvents) {
        if (event.getType() == EventType::RANDOM) {
            randomEvents.append(event);
        }
    }

    if (!randomEvents.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(randomEvents.size());
        randomEvents[randomIndex].showEventWindow();
    }

    for (const auto& event : triggerableEvents) {
        if (event.getType() != EventType::RANDOM) {
            event.showEventWindow();
            markEventOccurred(event.id);
        }
    }
}
