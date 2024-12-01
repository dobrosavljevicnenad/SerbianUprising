#include "client.h"
#include <QDebug>

Client::Client(QObject *parent)
    : QObject(parent), m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // readyRead i onReadyRead, kada stignu podaci
    // sa server automatski se povezuje sa metodom onReadyRead()

}

bool Client::connectToServer(const QString &hostAddress, quint16 port)
{
    m_socket->connectToHost(hostAddress, port);
    if (!m_socket->waitForConnected(5000)) {
        qWarning() << "Could not connect to server.";
        return false;
    }
    return true;
}

void Client::onReadyRead()
{
    QString data = QString::fromUtf8(m_socket->readAll());
    emit dataReceived(data);
}

void Client::sendData(const QString &data)
{
    if (m_socket->state() == QTcpSocket::ConnectedState) {
        // tekst u niz bajtova
        m_socket->write(data.toUtf8());
    }
}

void Client::onGameStarted()
{
    qDebug() << "Game started!";
    // Start the game logic here
}

void Client::onGameOver(const QString &reason)
{
    qDebug() << "Game over: " << reason;
    // Handle game over logic here
}

void Client::sendAction(const Action &action) {
    QString jsonData = action.toJson();
    sendData(jsonData);
}

void Client::sendEndTurn() {
    sendData("END_TURN");
}
