#include "turn.h"

Turn::Turn(Graph& graph) : currentPlayerId(1), m_graph(graph), moveArmy(graph),turn(1) {}

void Turn::addAction(int playerId, const Action& action) {
    auto& buffer = getPlayerBuffer(playerId);
    buffer.push_back(action);
}

void Turn::executeTurn() {
    auto& buffer = getPlayerBuffer(1);
    auto& buffer2 = getPlayerBuffer(2);
    bool battleMusic = 0;
    for (const auto& action : buffer) {
        if (action.type == ActionType::MOVE_ARMY){
            executeMoveAction(action);
        }
        if (action.type == ActionType::ATTACK){
            battleMusic = 1;
        }
    }

    for (const auto& action : buffer2) {
        if (action.type == ActionType::MOVE_ARMY){
            executeMoveAction(action);
        }

        if (action.type == ActionType::ATTACK){
            battleMusic = 1;
        }
    }

    std::time_t start_music = std::time(0);

    if(battleMusic){
        m_mediaPlayer.setAudioOutput(&m_audioOutput);
        m_audioOutput.setVolume(0.7);
        m_mediaPlayer.setSource(QUrl::fromLocalFile("../../resources/music/Volley.mp3"));

        m_mediaPlayer.play();

        QEventLoop loop;
        QTimer::singleShot(100, &loop, &QEventLoop::quit);
        loop.exec();
    }
    std::cout << "Executing Player 1's actions:\n";
    executePlayerActions(1);

    std::cout << "Executing Player 2's actions:\n";
    executePlayerActions(2);

    //while (std::difftime(std::time(0), start_music) < 6.7) {
    //}
    m_mediaPlayer.stop();

    turn+=1;
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

    if (!moveArmy.mergeArmies(source, target, action.soldiers)) {
        std::cerr << "Move action failed for Player " << action.playerId << ".\n";
    }
}

void Turn::executeAttackAction(const int playerId, const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    Vertex* target = m_graph.get_vertex_by_id(action.targetVertexId);
    std::vector<Vertex*> attackers;
    std::vector<unsigned> soldiers;
    attackers.push_back(source);
    auto& buffer = getPlayerBuffer(playerId);
    unsigned attackSoldiers = action.soldiers;
    if(source->army.getSoldiers()< action.soldiers)
        attackSoldiers = source->army.getSoldiers();
    soldiers.push_back(attackSoldiers);
    for (auto it = buffer.begin(); it != buffer.end(); ) {
        const auto& attackAction = *it;
        if (attackAction.type == ActionType::ATTACK && attackAction.targetVertexId == action.targetVertexId && attackAction.id != action.id) {
            Vertex* source = m_graph.get_vertex_by_id(attackAction.sourceVertexId);
            if (source != nullptr) {
                attackers.push_back(source);
                if (source->army.getSoldiers() < attackAction.soldiers) {
                    attackSoldiers = source->army.getSoldiers();
                    soldiers.push_back(attackSoldiers);
                }
                else
                    attackSoldiers = attackAction.soldiers;
                    soldiers.push_back(attackSoldiers);
            } else {
                std::cerr << "Error: Source vertex not found!" << std::endl;
            }

            it = buffer.erase(it);
        } else {
            ++it;
        }
    }

    if (!moveArmy.executeMove(attackers, target, soldiers)) {
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

