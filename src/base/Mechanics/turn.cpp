#include "turn.h"
#include <qapplication.h>
#include <qlabel.h>

Turn::Turn(Graph& graph) : m_graph(graph), moveArmy(graph), numBattles(0) {
    connect(&moveArmy, &MoveArmy::battleFinished, this, &Turn::onBattleFinished);
    connect(&moveArmy, &MoveArmy::battleCanceled, this, &Turn::onBattleCanceled);

}

void Turn::addAction(int playerId, const Action& action) {
    auto& buffer = getPlayerBuffer(playerId);
    buffer.push_back(action);
}

void Turn::executeTurn() {
    bool battleMusic = false;

    auto& buffer = getPlayerBuffer(1);
    auto& buffer2 = getPlayerBuffer(2);

    for (const auto& action : buffer) {
        if (action.type == ActionType::PLACE_ARMY){
            executePlaceAction(action);
        }
    }

    for (const auto& action : buffer2) {
        if (action.type == ActionType::PLACE_ARMY){
            executePlaceAction(action);
        }
    }

    // Execute actions for both players
    for (const auto& action : getPlayerBuffer(1)) {
        executePlayerMoves(action, battleMusic);
    }

    for (const auto& action : getPlayerBuffer(2)) {
        executePlayerMoves(action, battleMusic);
    }

    std::time_t start_music = std::time(0);
    // Play battle music if necessary
    if (battleMusic) {
        playBattleMusic();
    }
    // Execute actions for both players
    executePlayerAttacks(1);

    executePlayerAttacks(2);

    while (numBattles > 0) {
        QCoreApplication::processEvents(); // Ovo će omogućiti da se signali/slotovi procesuiraju dok čekate
    }
    for(auto res : battlesResults){
        res.printResults();
        std::cout << std::flush;
    }

    m_mediaPlayer.stop();
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

void Turn::executePlayerAttacks(int playerId) {
    auto& buffer = getPlayerBuffer(playerId);

    for (auto it = buffer.begin(); it != buffer.end(); ) {
        const auto& action = *it;
        switch (action.type) {
        case ActionType::ATTACK:
            numBattles++;
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

void Turn::executePlaceAction(const Action& action) {
    Vertex* source = m_graph.get_vertex_by_id(action.sourceVertexId);
    int soldiers = source->army.getSoldiers();
    source->army.setSoldiers(soldiers+action.soldiers);
    source->newRecruits = 0;
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

    if (!moveArmy.executeAttack(playerId, attackers, target, soldiers)) {
        std::cerr << "Attack action failed for Player " << action.playerId << ".\n";
    }
}

void Turn::onBattleFinished(Results results) {
    battlesResults.push_back(results);
    numBattles--;
    std::cout << "Battles left: " << numBattles << ".\n";
    std::cout << std::flush;

}
void Turn::onBattleCanceled(){
    numBattles--;
    std::cout << "Battles left: " << numBattles << ".\n";
    std::cout << std::flush;
}
QJsonObject Turn::serializeResultsVector(const std::vector<Results>& resultsVector) {
    QJsonObject jsonObject;
    QJsonArray resultsArray;

    for (const Results& result : resultsVector) {
        resultsArray.append(result.toJson());
    }

    jsonObject["results"] = resultsArray;
    return jsonObject;
}

