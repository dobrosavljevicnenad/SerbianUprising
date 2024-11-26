#include "server.h"

Server::Server(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)), m_clientSocket(nullptr) {
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startServer(quint16 port) {
    // QHostAddress::Any -> slusa  sa bilo koje konekcije na tom portu
    if (!m_server->listen(QHostAddress::Any, port)) {
        qWarning() << "Server could not start on port" << port;
        return false;
    }
    qDebug() << "Server started on port" << port; // cekamo klijente
    return true;
}

void Server::onNewConnection() {
    if (m_clientSocket) {
        qWarning() << "Already connected to a client.";
        return;
    }

    m_clientSocket = m_server->nextPendingConnection(); // QTcpSocket
    // kad klijent posalje podatke, poziva se onReadyRead
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    // kad se klijent disc, socket se brise
    connect(m_clientSocket, &QTcpSocket::disconnected, m_clientSocket, &QTcpSocket::deleteLater);

    qDebug() << "Client connected!";
}

void Server::onReadyRead() {
    // poziva se kad klijent posalje podatke
    if (!m_clientSocket) return;
    // podaci se salju kao niz bajtova, pa mora convert
    QString data = QString::fromUtf8(m_clientSocket->readAll());
    emit dataReceived(data);
}

// server salje odgovor klijentu
void Server::sendData(const QString &data) {
    if (m_clientSocket) {
        m_clientSocket->write(data.toUtf8());
    }
}
