#ifndef ACTION_H
#define ACTION_H

#include <atomic>

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
};

static std::atomic<int> nextId;

#endif // ACTION_H
