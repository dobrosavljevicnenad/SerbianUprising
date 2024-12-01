#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include "../base/Mechanics/Action.h"

Server::Server(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)), m_clientSocket(nullptr), m_secondPlayerSocket(nullptr), m_gameStarted(false), m_waitingForSecondPlayer(false), turn(g)
{
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startServer(quint16 port)
{
    // QHostAddress::Any -> slusa  sa bilo koje konekcije na tom portu
    if (!m_server->listen(QHostAddress::Any, port)) {
        qWarning() << "Server could not start on port" << port;
        return false;
    }
    qDebug() << "Server started on port" << port; // cekamo klijente
    return true;
    return true;
}

void Server::onNewConnection()
{
    // If the game has already started and we're waiting for second player
    if (m_gameStarted && m_waitingForSecondPlayer) {
        if (m_secondPlayerSocket == nullptr) {
            m_secondPlayerSocket = m_server->nextPendingConnection();// QTcpSocket
            // kad klijent posalje podatke, poziva se onReadyRead
            connect(m_secondPlayerSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
            // kad se klijent disc, socket se brise
            connect(m_secondPlayerSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
            qDebug() << "Second player reconnected!";
            emit gameStarted();  // Notify both players that game has started again
        }
        return;
    }

    // Handle the first player (host) connection
    if (m_clientSocket == nullptr) {
        m_clientSocket = m_server->nextPendingConnection();
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(m_clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Player 1 connected!";
    }
    // Handle the second player connection
    else if (m_secondPlayerSocket == nullptr) {
        m_secondPlayerSocket = m_server->nextPendingConnection();
        connect(m_secondPlayerSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(m_secondPlayerSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Player 2 connected!";
        m_gameStarted = true; // The game can now start
        emit gameStarted();  // Notify both players that game has started
    } else {
        qWarning() << "Maximum players already connected.";
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
                // Proveri da li je signal za End Turn
                if (data == "END_TURN") {
                    qDebug() << "End Turn received!";
                    if (socket == m_clientSocket) {
                        executeActions(actionsPlayer1);
                        actionsPlayer1.clear();
                    } else if (socket == m_secondPlayerSocket) {
                        executeActions(actionsPlayer2);
                        actionsPlayer2.clear();
                    }
                } else {
                    // Inače, tretiraj podatke kao Action
                    Action action = Action::fromJson(data);
                    if (socket == m_clientSocket) {
                        actionsPlayer1.push_back(action);
                    } else if (socket == m_secondPlayerSocket) {
                        actionsPlayer2.push_back(action);
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
        qDebug() << "Executing Action: " << action.toJson();
        // Ovde dodaj logiku za obradu akcija, npr. MOVE_ARMY, ATTACK, itd.
        if (action.type == ActionType::MOVE_ARMY) {
            std::cout << "VOJSKAAA SE POMERILA SEERVERRRRRRRRRRR" << std::endl;
        } else if (action.type == ActionType::ATTACK) {
            // Logika za napad
        }
    }
    qDebug() << "All actions executed for this turn.";
}



void Server::onClientDisconnected()
{
    if (m_clientSocket && sender() == m_clientSocket) {
        qDebug() << "Host (Player 1) disconnected!";
        m_clientSocket = nullptr;
        m_gameStarted = false;
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
