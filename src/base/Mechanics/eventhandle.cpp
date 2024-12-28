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

QJsonObject EventHandle::serializeProcessedEvents() {
    QJsonObject rootObject;
    QJsonArray eventsArray;

    for (const Event& event : processedEventsBuffer) {
        QJsonObject eventJson;
        eventJson["id"] = static_cast<int>(event.clientId);
        eventJson["title"] = event.title;

        eventsArray.append(eventJson);
    }

    processedEventsBuffer.clear();

    rootObject["events"] = eventsArray;
    return rootObject;
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

void EventHandle::processIntroEvents() {
    for (auto& eventPair : events) {
        Event& event = eventPair.first;
        if (event.trigger == "intro" && !eventPair.second) {
            event.showEventWindow();
            markEventOccurred(event.id);
        }
    }
}

void EventHandle::processSpecificEvent(int clientId, const QString& title,  AddArmyManager& armyManager, bool& naval ) {
    for (auto& eventPair : events) {
        Event& event = eventPair.first;
        if (event.clientId == clientId && event.title == title && !eventPair.second) {

            if(event.type != EventType::RANDOM){
                markEventOccurred(event.id);
            }

            qDebug() << "Displaying Event:" << event.title;

            event.updateOutcomeLabel();

            if (event.trigger == "recruitments") {
                processRecruitmentsTrigger(event, &armyManager);
                event.showEventWindow();
            } else if (event.trigger == "naval") {
                processNavalTrigger(naval,event);
                event.showEventWindow();
            } else if (event.trigger == "end") {
                isEnd = true;
                event.showEventWindow();
            } else {
                event.showEventWindow();
                qDebug() << "Event displayed but no specific action for trigger:" << event.trigger;
            }
            return;
        }
    }
    qWarning() << "No matching event found for ClientId:" << clientId << "Title:" << title;
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
        nonRandomEvents.append(randomEvent);
    }

    for (const Event& event : nonRandomEvents) {
        QString trigger = event.trigger;

        if (trigger == "place") {
            processPlaceTrigger(event, clientGraph,turn);
            processedEventsBuffer.append(event);
            markEventOccurred(event.id);
        } else if (trigger == "attack") {
            processAttackTrigger(event, clientGraph,turn);
            processedEventsBuffer.append(event);
            markEventOccurred(event.id);
        } else if (trigger == "morale") {
            processMoraleTrigger(event);
            processedEventsBuffer.append(event);
            markEventOccurred(event.id);
        } else if (trigger == "recruitments" || trigger == "naval" || trigger == "intro" || trigger == "end") {
            processedEventsBuffer.append(event);
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
            ActionType actionType = ActionType::EVENT_ATTACK;
            if(event.clientId == vertex->player.getPlayerId()){
                actionType = ActionType::PLACE_ARMY;
                qDebug() << "EventId:" << event.clientId<<"\nVertexId:" <<vertex->player.getPlayerId()<<"\nlabel:"<<vertex->label();

            }

            Action newAction(actionType, event.clientId, vertex->id(), vertex->id(), event.triggerAmount);
            turn->addAction(event.clientId, newAction);
            qDebug() << "Updated army strength in territory:" << label;
        }
    }
}

void EventHandle::processAttackTrigger(const Event& event, const graph::Graph& clientGraph,Turn* turn) {
    for (const QString& label : event.territoryAffect) {
        auto vertex = clientGraph.get_vertex_by_label(label);
        if (vertex) {
            ActionType actionType = ActionType::EVENT_ATTACK;
            if(event.clientId == vertex->player.getPlayerId())
                actionType = ActionType::PLACE_ARMY;
            Action newAction(actionType, event.clientId, vertex->id(), vertex->id(), event.triggerAmount);
            turn->addAction(event.clientId, newAction);
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

void EventHandle::processRecruitmentsTrigger(const Event& event, AddArmyManager* armyManager) {
    armyManager->addBonus(event.triggerAmount);
}

void EventHandle::processNavalTrigger(bool& naval, const Event& event) {
    naval = (event.triggerAmount != 0);
    qDebug() << "Naval strength updated to:" << naval;
}

void EventHandle::processEndTrigger(const Event& event) {
    qDebug() << "Game end event triggered:" << event.title;
    //triggerGameEndSequence(event);
}

bool EventHandle::shouldSpawnEvent(int probability) const {
    int randomValue = QRandomGenerator::global()->bounded(100);
    return randomValue < probability;
}
