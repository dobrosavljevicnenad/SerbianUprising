#include "turn.h"

Turn::Turn(Graph& graph) : currentPlayerId(1), m_graph(graph), moveArmy(graph),turn(1) {}

void Turn::addAction(int playerId, const Action& action) {
    auto& buffer = getPlayerBuffer(playerId);
    buffer.push_back(action);
}
void Turn::updateSoldiersForPlayer(int playerId) {
    auto& buffer = getPlayerBuffer(playerId);
    for (const auto& action : buffer) {
        auto vertex = m_graph.get_vertex_by_id(action.sourceVertexId);
        vertex->army.setSoldiers(vertex->army.getSoldiers() + action.soldiers);
    }
}
void Turn::executeTurn() {
    bool battleMusic = false;

    // Update the soldiers count for both players
    updateSoldiersForPlayer(1);
    updateSoldiersForPlayer(2);

    // Execute actions for both players
    for (const auto& action : getPlayerBuffer(1)) {
        executePlayerMoves(action, battleMusic);
    }
    for (const auto& action : getPlayerBuffer(2)) {
        executePlayerMoves(action, battleMusic);
    }

    // Play battle music if necessary
    if (battleMusic) {
        playBattleMusic();
    }

    // Execute actions for both players
    executePlayerAttacks(1);
    executePlayerAttacks(2);

    m_mediaPlayer.stop();
    turn++;
}

void Turn::executePlayerMoves(const Action& action, bool& battleMusic) {
    switch (action.type) {
    case ActionType::MOVE_ARMY:
        executeMoveAction(action);
        break;
    case ActionType::ATTACK:
        battleMusic = true;
        break;
    default:
        std::cerr << "Unknown action type!\n";
        break;
    }
}

void Turn::playBattleMusic() {
    std::time_t start_music = std::time(0);
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
    m_audioOutput.setVolume(0.7);
    m_mediaPlayer.setSource(QUrl::fromLocalFile("../../resources/music/Volley.mp3"));
    m_mediaPlayer.play();


    QEventLoop loop;
    QTimer::singleShot(100, &loop, &QEventLoop::quit);
    loop.exec();
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

void Turn::executePlayerAttacks(int playerId) {
    auto& buffer = getPlayerBuffer(playerId);

    for (auto it = buffer.begin(); it != buffer.end(); ) {
        const auto& action = *it;
        switch (action.type) {
        case ActionType::ATTACK:
            executeAttackAction(playerId, action);
            break;
        case ActionType::MOVE_ARMY:
            break;
        default:
            std::cerr << "Unknown action type!\n";
            break;
        }
        it = buffer.erase(it);

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

QString Turn::GetCurrentAction(const Action& action) {
    QString moveDescription = QString("%2 troops from Layer %3 to Layer %4")
    .arg(action.soldiers)
        .arg(action.sourceVertexId)
        .arg(action.targetVertexId);
    return moveDescription;
}

void Turn::executeMoveAction(const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    Vertex* target = m_graph.get_vertex_by_id(action.targetVertexId);
    if (!moveArmy.executeMerge(source, target, action.soldiers)) {
        std::cerr << "Merge action failed for Player " << action.playerId << ".\n";
    }
}

void Turn::executeAttackAction(const int playerId, const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    Vertex* target = m_graph.get_vertex_by_id(action.targetVertexId);

    std::vector<Vertex*> attackers = {source};
    std::vector<unsigned> soldiers = {static_cast<unsigned int>(std::min(source->army.getSoldiers(), action.soldiers))};

    auto& buffer = getPlayerBuffer(playerId);
    for (auto it = buffer.begin(); it != buffer.end(); ) {
        const auto& attackAction = *it;
        if (attackAction.type == ActionType::ATTACK && attackAction.targetVertexId == action.targetVertexId && attackAction.id != action.id) {
            Vertex* attackSource = m_graph.get_vertex_by_id(attackAction.sourceVertexId);
            if (attackSource != nullptr) {
                attackers.push_back(attackSource);
                soldiers.push_back(std::min(attackSource->army.getSoldiers(), attackAction.soldiers));
            } else {
                std::cerr << "Error: Source vertex not found!" << std::endl;
            }
            it = buffer.erase(it); // Remove the attack action from the buffer
        } else {
            ++it;
        }
    }

    if (!moveArmy.executeAttack(attackers, target, soldiers)) {
        std::cerr << "Attack action failed for Player " << action.playerId << ".\n";
    }
}

void Turn::removeActionById(int actionId) {
    auto& buffer = getPlayerBuffer(currentPlayerId);

    auto it = std::find_if(buffer.begin(), buffer.end(),
                           [actionId](const Action& action) {
                               return action.id == actionId;
                           });

    if (it != buffer.end()) {
        ///updates soldier count
        graph::Vertex* cvor = m_graph.get_vertex_by_id(it->sourceVertexId);
        cvor->army.setSoldiers(cvor->army.getSoldiers() + it->soldiers);
        cvor->map_layer->setTroopCount(cvor->army.getSoldiers());
        ///
        buffer.erase(it);
        std::cout << "Action with ID " << actionId << " removed for Player " << currentPlayerId << ".\n";
    } else {
        std::cerr << "Action with ID " << actionId << " not found for Player " << currentPlayerId << ".\n";
    }
}

int Turn::getTurn(){
    return turn;
}

