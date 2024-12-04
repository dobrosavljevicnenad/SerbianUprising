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

void Client::setId(int newId) {
    id = newId;
    qDebug() << "Client ID set to:" << id;
}

int Client::getId() const {
    return id;
}

void Client::onReadyRead() {
    QByteArray rawData = m_socket->readAll();
    QString data = QString::fromUtf8(rawData).trimmed();

    // Split the data by newline to handle multiple messages
    QStringList messages = data.split("\n", Qt::SkipEmptyParts);
    for (const QString& message : messages) {
        if (message.startsWith("ID:")) {
            bool ok;
            int receivedId = message.mid(3).toInt(&ok);
            if (ok) {
                id = receivedId;
                emit idReceived(id);  // Emit the ID signal
            } else {
                qWarning() << "Failed to parse ID from server message:" << message;
            }
        } else if (message == "START_GAME") {
            emit gameStarted();  // Emit game started signal
        } else {
            emit dataReceived(message);  // Emit general data signal
        }
    }
}

void Client::disconnectFromServer() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(3000); // Wait for disconnection
        }
    }
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


