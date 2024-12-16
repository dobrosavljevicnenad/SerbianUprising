#include "client.h"
#include <QDebug>

Client::Client(QObject *parent)
    : QObject(parent),
    m_socket(new QTcpSocket(this)),
    clientGameManager(new ClientGameManager(nullptr, this))
{
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // readyRead i onReadyRead, kada stignu podaci
    // sa server automatski se povezuje sa metodom onReadyRead()
    connect(clientGameManager, &ClientGameManager::endTurnActionsReady, this, &Client::sendEndTurnWithActions);
    connect(clientGameManager, &ClientGameManager::gameDataLoaded, this, &Client::handleLoadGame);
}
ClientGameManager* Client::getClientGameManager() const {
    return clientGameManager;
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
                clientGameManager->setId(id);
                emit idReceived(id);  // Emit the ID signal
            } else {
                qWarning() << "Failed to parse ID from server message:" << message;
            }
        } else if (message == "START_GAME") {
            emit gameStarted();  // Emit game started signal
        } else {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();

                // Proveravamo tip poruke
                QString type = jsonObject["type"].toString();
                if (type == "GAME_DATA") {
                    QJsonObject gameData = jsonObject["gameData"].toObject();
                    clientGameManager->processLoadData(gameData);  // Emitujemo signal sa podacima igre
                    qDebug() << "Received GAME_DATA from server:" << gameData;
                } else {
                    clientGameManager->processDataFromServer(jsonObject);
                }
            } else {
                qWarning() << "Failed to parse JSON data from server:" << message;
            }
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

void Client::sendEndTurnWithActions(const QVector<Action> &actions, int id) {
    QJsonArray actionsArray;
    for (const Action &action : actions) {
        actionsArray.append(QJsonDocument::fromJson(action.toJson().toUtf8()).object());
    }

    QJsonObject jsonObject;
    jsonObject["type"] = "END_TURN";
    jsonObject["id"] = id;
    jsonObject["actions"] = actionsArray;

    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact); // No double serialization
    qDebug() << "Sending JSON to server:" << jsonString;
    m_socket->write(jsonString.toUtf8());
}

void Client::handleLoadGame(const QJsonObject& graphData) {
    if (!m_socket || !m_socket->isOpen()) {
        qWarning() << "Socket is not open. Cannot send game data to server.";
        return;
    }

    QJsonObject message;
    message["type"] = "LOAD_GAME";
    message["gameData"] = graphData;

    QJsonDocument jsonDoc(message);
    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);

    m_socket->write(jsonData + "\n");
    m_socket->flush();

    qDebug() << "Sent game data to server:" << jsonData;
}




// klijent salje serveru action i ID klijenta
// server primi i skladisti podatke
