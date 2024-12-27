#include "eventhandle.h"

EventHandle::EventHandle() {}

void EventHandle::deserializeEvents(const QJsonArray& eventsJson, unsigned clientId) {
    unsigned int idCounter = 0;
    for (const QJsonValue& value : eventsJson) {
        if (value.isObject()) {
            QJsonObject eventJson = value.toObject();
            unsigned id = eventJson["id"].toInt();

            if (clientId != 0 && clientId != id) continue;

            EventType type = Event::stringToEventType(eventJson["type"].toString().toStdString());
            QString title = eventJson["title"].toString();
            QString imagePath = eventJson["image_path"].toString();
            QString description = eventJson["description"].toString();
            QString buttonText = eventJson["button"].toString();
            QVector<QString> territoryTrigger;
            if (eventJson.contains("territory_trigger") && eventJson["territory_trigger"].isArray()) {
                for (const QJsonValue& value : eventJson["territory_trigger"].toArray()) {
                    territoryTrigger.append(value.toString());
                }
            }
            QString trigger = eventJson["trigger"].toString();
            int triggerAmount = eventJson["trigger_amount"].toInt();
            QVector<QString> territoryAffect;
            if (eventJson.contains("territory_affect") && eventJson["territory_affect"].isArray()) {
                for (const QJsonValue& value : eventJson["territory_affect"].toArray()) {
                    territoryAffect.append(value.toString());
                }
            }
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
    qDebug() << currentYear;
    QVector<Event> randomEvents;
    QVector<Event> nonRandomEvents;

    // Separate RANDOM and non-RANDOM events that can be triggered
    for (const auto& eventPair : events) {
        const Event& event = eventPair.first;
        if (!eventPair.second && event.canTrigger(clientId, currentYear, clientGraph)) {
            if (event.getType() == EventType::RANDOM) {
                if (shouldSpawnEvent(20) && currentYear != "01-01-1804") { // Only include RANDOM events that pass the spawn check
                    randomEvents.append(event);
                }
            } else {
                nonRandomEvents.append(event);
            }
        }
    }

    // Process RANDOM Events
    if (!randomEvents.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(randomEvents.size());
        Event& randomEvent = randomEvents[randomIndex];
        randomEvent.showEventWindow();
    }

    // Process Non-RANDOM Events
    for (auto& event : nonRandomEvents) {
        event.showEventWindow();
        markEventOccurred(event.id); // Mark each displayed non-RANDOM event as occurred
    }
}

bool EventHandle::shouldSpawnEvent(int probability) const {
    int randomValue = QRandomGenerator::global()->bounded(100);
    return randomValue < probability;
}
