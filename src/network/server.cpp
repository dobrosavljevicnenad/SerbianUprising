#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include "../base/Mechanics/Action.h"

// Constructor
Server::Server(QObject *parent)
    : QObject(parent),
    m_server(new QTcpServer()),
    m_clientSocket(nullptr),
    m_secondPlayerSocket(nullptr),
    m_waitingForSecondPlayer(false),
    serverGameManager(new ServerGameManager(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    connect(this, &Server::startGame, this, &Server::onGameStartRequested);

    connect(serverGameManager, &ServerGameManager::init_serializedGraphReady, this, &Server::handleSerializedGraph_init);
    connect(serverGameManager, &ServerGameManager::serializedGraphReady2, this, &Server::handleSerializedGraph);
}

// Public Methods
ServerGameManager* Server::getGameManager() {
    return serverGameManager;
}

bool Server::startServer(quint16 port)
{
    if (!m_server->listen(QHostAddress::Any, port)) {
        qWarning() << "Server could not start on port" << port;
        return false;
    }

    qDebug() << "Server started on port" << port;
    return true;
}

// Slots - Connections
void Server::onNewConnection() {
    QTcpSocket* newSocket = m_server->nextPendingConnection();

    if (!newSocket) {
        qWarning() << "Failed to establish a new connection.";
        return;
    }

    if (m_clientSocket == nullptr) {
        m_clientSocket = newSocket;
        setupPlayerSocket(m_clientSocket, "Player 1", "ID:1\n");
    }
    else if (m_secondPlayerSocket == nullptr) {
        m_secondPlayerSocket = newSocket;
        setupPlayerSocket(m_secondPlayerSocket, "Player 2", "ID:2\n");

        if (m_clientSocket && m_secondPlayerSocket) {
            qDebug() << "Both players connected. Starting game.";
            broadcast("START_GAME");
            emit startGame();
        }
    }
    else {
        qWarning() << "Maximum players already connected.";
        newSocket->disconnectFromHost();
    }
}

void Server::onGameStartRequested() {
    serverGameManager->startGame();
    // prosiri logiku emituje ... nesto
}

void Server::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        qWarning() << "Invalid sender in onReadyRead";
        return;
    }

    while (socket->bytesAvailable() > 0) {
        QByteArray rawData = socket->readLine().trimmed();
        if (rawData.isEmpty()) continue;

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData, &parseError);

        if (parseError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
            qWarning() << "Invalid JSON received:" << rawData;
            continue;
        }

        QJsonObject jsonObject = jsonDoc.object();
        QString type = jsonObject["type"].toString();

        if (type == "END_TURN") {
            handleEndTurn(jsonObject);
        } else if (type == "LOAD_GAME") {
            handleLoadGameRequest(jsonObject);
        } else {
            qWarning() << "Unknown message type received:" << type;
        }
    }
}

void Server::onClientDisconnected() {
    if (m_clientSocket && sender() == m_clientSocket) {
        qDebug() << "Host (Player 1) disconnected!";
        m_clientSocket = nullptr;
        emit gameOver("Host left, game over.");
    }
    else if (m_secondPlayerSocket && sender() == m_secondPlayerSocket) {
        qDebug() << "Second player disconnected!";
        m_secondPlayerSocket = nullptr;
        m_waitingForSecondPlayer = true;
        emit gameOver("Second player left, waiting for reconnection.");
    }
}

// Private Methods
void Server::setupPlayerSocket(QTcpSocket* socket, const QString& playerName, const QString& message) {
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << playerName << "connected!";
    socket->write(message.toUtf8());
    socket->flush();
}

void Server::broadcast(const QString &message) {
    if (m_secondPlayerSocket && m_secondPlayerSocket->state() == QAbstractSocket::ConnectedState) {
        m_secondPlayerSocket->write(message.toUtf8() + "\n");
        m_secondPlayerSocket->flush();
    }
    if (m_clientSocket && m_clientSocket->state() == QAbstractSocket::ConnectedState) {
        m_clientSocket->write(message.toUtf8() + "\n");
        m_clientSocket->flush();
    }
}

void Server::sendData(const QString &data) {
    if (m_clientSocket) m_clientSocket->write(data.toUtf8());
    if (m_secondPlayerSocket) m_secondPlayerSocket->write(data.toUtf8());
}

void Server::executeActions(const std::vector<Action> &actions) {
    for (const Action &action : actions) {
        // Placeholder for actions
    }
}

// Handlers
void Server::handleSerializedGraph_init(const QJsonObject &serializedGraph) {
    QJsonObject dataToSend;
    dataToSend["graph"] = serializedGraph;

    QString serializedData = QString(QJsonDocument(dataToSend).toJson(QJsonDocument::Compact));
    sendData(serializedData);
}

void Server::handleSerializedGraph(const QJsonObject& serializedGraph, const QJsonObject& results) {
    QJsonObject dataToSend;
    dataToSend["graph"] = serializedGraph;
    dataToSend["results"] = results;

    QString serializedData = QString(QJsonDocument(dataToSend).toJson(QJsonDocument::Compact));
    sendData(serializedData);
}

void Server::handleEndTurn(const QJsonObject &jsonObject) {
    int id = jsonObject["id"].toInt();
    QJsonArray actionsArray = jsonObject["actions"].toArray();
    QVector<Action> actions;

    for (const QJsonValue &value : actionsArray) {
        QJsonObject obj = value.toObject();
        QString jsonString = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        actions.push_back(Action::fromJson(jsonString));
    }

    endTurnActions[id] = actions;

    if (endTurnActions.size() == 2) {
        auto it = endTurnActions.begin();
        int p1_id = it.key();
        std::vector<Action> actionsPlayer1(it.value().begin(), it.value().end());

        ++it;
        int p2_id = it.key();
        std::vector<Action> actionsPlayer2(it.value().begin(), it.value().end());

        serverGameManager->executeActions(actionsPlayer1, p1_id, actionsPlayer2, p2_id);
        endTurnActions.clear();
    }
}

void Server::handleLoadGameRequest(const QJsonObject &jsonObject) {
    qDebug() << "Received LOAD_GAME request.";

    if (jsonObject.contains("gameData")) {
        QJsonObject gameData = jsonObject["gameData"].toObject();
        handleLoadGame(gameData);
    } else {
        qWarning() << "LOAD_GAME request is missing 'gameData'.";
    }
}

void Server::handleLoadGame(const QJsonObject &gameData) {
    QJsonObject response;
    response["type"] = "GAME_DATA";
    response["gameData"] = gameData;

    QJsonDocument responseDoc(response);
    QByteArray responseData = responseDoc.toJson(QJsonDocument::Compact);

    sendData(responseData);
    qDebug() << "Sent game data to all clients:" << responseData;
}
