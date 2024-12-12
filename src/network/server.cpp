#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include "../base/Mechanics/Action.h"

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
    connect(serverGameManager, &ServerGameManager::serializedGraphReady, this, &Server::handleSerializedGraph);
}

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

void Server::onNewConnection() {
    QTcpSocket* newSocket = m_server->nextPendingConnection();

    if (!newSocket) {
        qWarning() << "Failed to establish a new connection.";
        return;
    }

    if (m_clientSocket == nullptr) {
        m_clientSocket = newSocket;
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(m_clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Player 1 connected!";
        m_clientSocket->write("ID:1\n");
        m_clientSocket->flush();
    } else if (m_secondPlayerSocket == nullptr) {
        m_secondPlayerSocket = newSocket;
        connect(m_secondPlayerSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(m_secondPlayerSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Player 2 connected!";
        m_secondPlayerSocket->write("ID:2\n");
        m_secondPlayerSocket->flush();

        if (m_clientSocket && m_secondPlayerSocket) {
            qDebug() << "Both players connected. Starting game.";
            broadcast("START_GAME");

            emit startGame();
        }
    } else {
        qWarning() << "Maximum players already connected.";
        newSocket->disconnectFromHost();
    }
}
void Server::onGameStartRequested(){
    serverGameManager->startGame();
    //prosiri logiku emituje ... nesto
}

void Server::broadcast(const QString &message) {
    if (m_secondPlayerSocket && m_secondPlayerSocket->state() == QAbstractSocket::ConnectedState) {
        m_secondPlayerSocket->write(message.toUtf8());
        m_secondPlayerSocket->flush();
    }
    if (m_clientSocket && m_clientSocket->state() == QAbstractSocket::ConnectedState) {
        m_clientSocket->write(message.toUtf8());
        m_clientSocket->flush();
    }
}

void Server::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        qWarning() << "Invalid sender in onReadyRead";
        return;
    }
    if (socket->bytesAvailable() > 0) {
        QString data = QString::fromUtf8(socket->readAll()).trimmed();

        if (!data.isEmpty()) {
            try {
                QJsonObject jsonObject = QJsonDocument::fromJson(data.toUtf8()).object();
                if (jsonObject["type"] == "END_TURN") {
                    int id = jsonObject["id"].toInt();
                    QJsonArray actionsArray = jsonObject["actions"].toArray();
                    QVector<Action> actions;

                    for (const QJsonValue &value : actionsArray) {
                        QJsonObject obj = value.toObject();
                        QString jsonString = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
                        actions.push_back(Action::fromJson(jsonString));
                    }

                    endTurnActions[id] = actions;

                    // Provera da li su oba igraca zavrsila potez
                    if (endTurnActions.size() == 2) {
                        auto it = endTurnActions.begin();
                        int p1_id = it.key();
                        QVector<Action> actionsPlayer1QVector = it.value();
                        std::vector<Action> actionsPlayer1(actionsPlayer1QVector.begin(), actionsPlayer1QVector.end());
                        ++it;
                        int p2_id = it.key();
                        QVector<Action> actionsPlayer2QVector = it.value();
                        std::vector<Action> actionsPlayer2(actionsPlayer2QVector.begin(), actionsPlayer2QVector.end());

                        serverGameManager->executeActions(actionsPlayer1, p1_id, actionsPlayer2, p2_id);
                        // cistimo endturn-ove za sledece poteze
                        endTurnActions.clear();
                    }

                }
            } catch (std::exception &e) {
                qWarning() << "Failed to parse data:" << e.what();
            }
        }
    }
}

void Server::executeActions(const std::vector<Action> &actions) {
    for (const Action &action : actions) {
    }
}

void Server::onClientDisconnected()
{
    if (m_clientSocket && sender() == m_clientSocket) {
        qDebug() << "Host (Player 1) disconnected!";
        m_clientSocket = nullptr;
        emit gameOver("Host left, game over.");
    } else if (m_secondPlayerSocket && sender() == m_secondPlayerSocket) {
        qDebug() << "Second player disconnected!";
        m_secondPlayerSocket = nullptr;
        m_waitingForSecondPlayer = true;  // Now we're waiting for the second player to reconnect
        emit gameOver("Second player left, waiting for reconnection.");
    }
}

// server salje odgovor klijentu
void Server::sendData(const QString &data)
{
    if (m_clientSocket) {
        m_clientSocket->write(data.toUtf8());
    }
    if (m_secondPlayerSocket) {
        m_secondPlayerSocket->write(data.toUtf8());
    }
}

void Server::handleSerializedGraph(const QJsonObject &serializedGraph) {
    QString serializedData = QString(QJsonDocument(serializedGraph).toJson(QJsonDocument::Compact));
    sendData(serializedData); // send to clients
}
