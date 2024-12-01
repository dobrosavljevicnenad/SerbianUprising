#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include "../base/Mechanics/Action.h"

Server::Server(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)), m_clientSocket(nullptr), m_secondPlayerSocket(nullptr), m_gameStarted(false), m_waitingForSecondPlayer(false)
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
                Action action = Action::fromJson(data);
                qDebug() << "[SERVER]Received Action:" << action.toJson();
                socket->write(QString("Action received: %1\n").arg(data).toUtf8());

                // Dalja logika za obradu akcije...
                emit dataReceived(data); // Emit signal if needed
            } catch (std::exception &e) {
                qWarning() << "Failed to parse Action:" << e.what();
            }
        } else {
            qDebug() << "Received empty data, ignoring.";
        }
    }
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
