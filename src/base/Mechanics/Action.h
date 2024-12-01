#ifndef ACTION_H
#define ACTION_H

#include <atomic>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>

enum class ActionType { MOVE_ARMY, ATTACK};

struct Action {
    static std::atomic<int> nextId; //Thread Safe for later impl and now but automatic increment
    ActionType type;       // Type of action
    int id;                // Unique ID for the action
    int playerId;          // ID of the player performing the action
    int sourceVertexId;    // Source vertex ID (for MOVE_ARMY or ATTACK)
    int targetVertexId;    // Target vertex ID (for MOVE_ARMY or ATTACK)
    int soldiers;          // Number of soldiers involved in the action

    Action(ActionType type, int playerId, int source, int target, int soldiers)
        : id(nextId++), type(type), playerId(playerId), sourceVertexId(source),
        targetVertexId(target), soldiers(soldiers) {}

    friend std::ostream& operator<<(std::ostream& os, const Action& action) {
        os << "Action ID: " << action.id
           << ", Type: " << static_cast<int>(action.type) // assuming ActionType is an enum
           << ", Player ID: " << action.playerId
           << ", Source Vertex ID: " << action.sourceVertexId
           << ", Target Vertex ID: " << action.targetVertexId
           << ", Soldiers: " << action.soldiers;
        return os;
    }

    QString toJson() const {
        QJsonObject json;
        json["type"] = static_cast<int>(type);
        json["id"] = id;
        json["playerId"] = playerId;
        json["sourceVertexId"] = sourceVertexId;
        json["targetVertexId"] = targetVertexId;
        json["soldiers"] = soldiers;
        return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
    }

    // Deserialize Action from JSON
    static Action fromJson(const QString &jsonString) {
        QJsonObject json = QJsonDocument::fromJson(jsonString.toUtf8()).object();
        Action action(static_cast<ActionType>(json["type"].toInt()),
                      json["playerId"].toInt(),
                      json["sourceVertexId"].toInt(),
                      json["targetVertexId"].toInt(),
                      json["soldiers"].toInt());
        action.id = json["id"].toInt(); // Override auto-increment if needed
        return action;
    }
};

static std::atomic<int> nextId;

#endif // ACTION_H
