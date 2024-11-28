#ifndef ACTION_H
#define ACTION_H

enum class ActionType { MOVE_ARMY, ATTACK };

struct Action {
    ActionType type;       // Type of action
    int id;                // Unique ID for the action
    int playerId;          // ID of the player performing the action
    int sourceVertexId;    // Source vertex ID (for MOVE_ARMY or ATTACK)
    int targetVertexId;    // Target vertex ID (for MOVE_ARMY or ATTACK)
    int soldiers;          // Number of soldiers involved in the action

    // Static counter to ensure unique IDs
    static int nextId;  // Declaration only, no definition here.

    Action(ActionType type, int playerId, int source, int target, int soldiers)
        : type(type), playerId(playerId), sourceVertexId(source), targetVertexId(target), soldiers(soldiers) {
        id = nextId++;  // Assign a unique ID and then increment
    }
};

#endif // ACTION_H
