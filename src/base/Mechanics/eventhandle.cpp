#include "eventhandle.h"

EventHandle::EventHandle() {}

void EventHandle::deserializeEvents(const QJsonArray& eventsJson, unsigned clientId) {
    unsigned int idCounter = 0;
    for (const QJsonValue& value : eventsJson) {
        if (value.isObject()) {
            QJsonObject eventJson = value.toObject();
            int id = eventJson["id"].toInt();

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

            Event event(idCounter++,id, type, title, imagePath, description, buttonText,
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

void EventHandle::processEvents(const QString& currentYear, const graph::Graph& clientGraph, Turn* turn) {
    QVector<Event> randomEvents;
    QVector<Event> nonRandomEvents;

    for (const auto& eventPair : events) {
        const Event& event = eventPair.first;
        if (!eventPair.second && event.canTrigger(currentYear, clientGraph)) {
            if (event.getType() == EventType::RANDOM) {
                if (shouldSpawnEvent(20) && currentYear != "01-01-1804") {
                    randomEvents.append(event);
                }
            } else {
                nonRandomEvents.append(event);
            }
        }
    }

    if (!randomEvents.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(randomEvents.size());
        Event& randomEvent = randomEvents[randomIndex];
        processRandomEvent(randomEvent);
    }

    for (const Event& event : nonRandomEvents) {
        QString trigger = event.trigger;

        if (trigger == "place") {
            processPlaceTrigger(event, clientGraph, turn);
            markEventOccurred(event.id);
        } else if (trigger == "attack") {
            processAttackTrigger(event, clientGraph, turn);
            markEventOccurred(event.id);
        } else if (trigger == "morale") {
            processMoraleTrigger(event);
            markEventOccurred(event.id);
        } else if (trigger == "recruitments" || trigger == "naval" || trigger == "intro" || trigger == "end") {
            markEventOccurred(event.id);
        } else {
            qWarning() << "Unknown trigger type:" << trigger;
        }
    }
}

void EventHandle::processPlaceTrigger(const Event& event, const graph::Graph& clientGraph, Turn* turn) {
    for (const QString& label : event.territoryAffect) {
        auto vertex = clientGraph.get_vertex_by_label(label);
        if (vertex) {
            Action newAction(ActionType::PLACE_ARMY, clientId, vertex->id(), vertex->id(), event.triggerAmount);
            turn->addAction(clientId, newAction);
            qDebug() << "Updated army strength in territory:" << label;
        }
    }
}

void EventHandle::processAttackTrigger(const Event& event, const graph::Graph& clientGraph,Turn* turn) {
    for (const QString& label : event.territoryAffect) {
        auto vertex = clientGraph.get_vertex_by_label(label);
        if (vertex) {
            Action newAction(ActionType::EVENT_ATTACK, clientId, vertex->id(), vertex->id(), event.triggerAmount);
            turn->addAction(clientId, newAction);
            qDebug() << "Marked territory as under attack:" << label;
        }
    }
}

void EventHandle::processMoraleTrigger(const Event& event) {
    int moraleChange = event.triggerAmount;
    if(clientId == 1)
        Strength::instance().setBoost(ArmyType::HAJDUK, moraleChange);
    else if(clientId == 2)
        Strength::instance().setBoost(ArmyType::JANISSARY, moraleChange);
    qDebug() << "Global morale updated by:" << moraleChange;
}

void EventHandle::processRecruitmentsTrigger(const Event& event, const graph::Graph& clientGraph) {
    for (const QString& label : event.territoryAffect) {
        auto vertex = clientGraph.get_vertex_by_label(label);
        if (vertex) {

            qDebug() << "Recruited units in territory:" << label;
        }
    }
}

void EventHandle::processNavalTrigger(const Event& event) {
    qDebug() << "Naval strength updated by:";
}

void EventHandle::processIntroTrigger(const Event& event) {
    qDebug() << "Game intro event triggered:" << event.title;
    //(event);
}

void EventHandle::processEndTrigger(const Event& event) {
    qDebug() << "Game end event triggered:" << event.title;
    //triggerGameEndSequence(event);
}

void EventHandle::processRandomEvent(const Event& event) {
    qDebug() << "Random event triggered:" << event.title;
}

bool EventHandle::shouldSpawnEvent(int probability) const {
    int randomValue = QRandomGenerator::global()->bounded(100);
    return randomValue < probability;
}
