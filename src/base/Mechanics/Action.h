#ifndef ACTION_H
#define ACTION_H

enum class ActionType { MOVE_ARMY, ATTACK };

struct Action {
    ActionType type;       // Type of action
    int playerId;          // ID of the player performing the action
    int sourceVertexId;    // Source vertex ID (for MOVE_ARMY or ATTACK)
    int targetVertexId;    // Target vertex ID (for MOVE_ARMY or ATTACK)
    int soldiers;          // Number of soldiers involved in the action

    Action(ActionType type, int playerId, int source, int target, int soldiers)
        : type(type), playerId(playerId), sourceVertexId(source), targetVertexId(target), soldiers(soldiers) {}
};

#endif // ACTION_H
