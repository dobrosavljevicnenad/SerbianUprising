#include "turn.h"
#include "MoveArmy.h"
#include <sstream>  // For std::stringstream
Turn::Turn(Graph& graph) : currentPlayerId(1), m_graph(graph), moveArmy(graph) {}

void Turn::addAction(int playerId, const Action& action) {
    auto& buffer = getPlayerBuffer(playerId);
    buffer.push_back(action);
}

void Turn::executeTurn() {
    std::cout << "Executing Player 1's actions:\n";
    executePlayerActions(1);

    std::cout << "Executing Player 2's actions:\n";
    executePlayerActions(2);
}

void Turn::clearActionBuffers() {
    player1Buffer.clear();
    player2Buffer.clear();
}

void Turn::changePlayer() {
    currentPlayerId = (currentPlayerId == 1) ? 2 : 1;
    std::cout << "Switched to Player " << currentPlayerId << ".\n";
}

int Turn::getCurrentPlayerId() const {
    return currentPlayerId;
}

void Turn::executePlayerActions(int playerId) {
    auto& buffer = getPlayerBuffer(playerId);

    for (const auto& action : buffer) {
        switch (action.type) {
        case ActionType::MOVE_ARMY:
            executeMoveAction(action);
            break;
        case ActionType::ATTACK:
            executeAttackAction(action);
            break;
        default:
            std::cerr << "Unknown action type!\n";
            break;
        }
    }

    buffer.clear(); // Clear the buffer after executing all actions
}

std::vector<Action>& Turn::getPlayerBuffer(int playerId) {
    if (playerId == 1) {
        return player1Buffer;
    } else if (playerId == 2) {
        return player2Buffer;
    } else {
        throw std::invalid_argument("Invalid player ID");
    }
}
QString Turn::bufferToString() {
    std::stringstream ss;

    // Convert player 1's actions
    ss << "Player 1's Actions:\n";
    for (const auto& action : player1Buffer) {
        ss << "Action: ";
        ss << (action.type == ActionType::MOVE_ARMY ? "Move Army" : "Attack") << ", ";
        ss << "Source: " << action.sourceVertexId << ", ";
        ss << "Target: " << action.targetVertexId << ", ";
        ss << "Soldiers: " << action.soldiers << "\n";
    }

    // Convert player 2's actions
    ss << "\nPlayer 2's Actions:\n";
    for (const auto& action : player2Buffer) {
        ss << "Action: ";
        ss << (action.type == ActionType::MOVE_ARMY ? "Move Army" : "Attack") << ", ";
        ss << "Source: " << action.sourceVertexId << ", ";
        ss << "Target: " << action.targetVertexId << ", ";
        ss << "Soldiers: " << action.soldiers << "\n";
    }

    // Convert to QString and return
    return QString::fromStdString(ss.str());
}
void Turn::executeMoveAction(const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    Vertex* target = m_graph.get_vertex_by_id(action.targetVertexId);

    if (!moveArmy.executeMove(source, target, action.soldiers)) {
        std::cerr << "Move action failed for Player " << action.playerId << ".\n";
    }
}

void Turn::executeAttackAction(const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    Vertex* target = m_graph.get_vertex_by_id(action.targetVertexId);

    if (!moveArmy.executeMove(source, target, action.soldiers)) {
        std::cerr << "Attack action failed for Player " << action.playerId << ".\n";
    }
}
