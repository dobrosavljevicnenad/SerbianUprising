#include "client.h"
#include <QDebug>

// -------------------- Constructor --------------------

Client::Client(QObject *parent)
    : QObject(parent),
    m_socket(new QTcpSocket(this)),
    clientGameManager(new ClientGameManager(nullptr, this)) {
    setupConnections();
}

// -------------------- Private methods --------------------

void Client::setupConnections() {
    // Povezivanje signala i slotova
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(clientGameManager, &ClientGameManager::endTurnActionsReady, this, &Client::sendEndTurnWithActions);
    connect(clientGameManager, &ClientGameManager::gameDataLoaded, this, &Client::handleLoadGame);
}

// -------------------- Getters and setters --------------------

ClientGameManager* Client::getClientGameManager() const {
    return clientGameManager;
}

void Client::setId(int newId) {
    id = newId;
}

int Client::getId() const {
    return id;
}

// -------------------- Connect with server --------------------

bool Client::connectToServer(const QString &hostAddress, quint16 port) {
    m_socket->connectToHost(hostAddress, port);
    if (!m_socket->waitForConnected(5000)) {
        qWarning() << "Could not connect to server.";
        return false;
    }
    return true;
}

void Client::disconnectFromServer() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(3000);
        }
    }
}

// -------------------- Send Data --------------------

void Client::sendData(const QString &data) {
    QByteArray jsonData = data.toUtf8();

    QByteArray message = jsonData + "\n\n";

    m_socket->write(message);
    m_socket->flush();
}

void Client::sendEndTurnWithActions(const QVector<Action> &actions, int id) {
    QJsonObject jsonObject = prepareEndTurnMessage(actions, id);
    QString jsonString = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);

    QByteArray toSend = jsonString.toUtf8() + "\n\n";
    m_socket->write(toSend);
    m_socket->flush();
}

QJsonObject Client::prepareEndTurnMessage(const QVector<Action> &actions, int id) const {
    QJsonArray actionsArray;
    for (const Action &action : actions) {
        actionsArray.append(QJsonDocument::fromJson(action.toJson().toUtf8()).object());
    }

    QJsonObject jsonObject;
    jsonObject["type"] = "END_TURN";
    jsonObject["id"] = id;
    jsonObject["actions"] = actionsArray;

    return jsonObject;
}

void Client::handleLoadGame(const QJsonObject& graphData)
{
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


// -------------------- Public IP --------------------

void Client::getPublicIp() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Client::onPublicIpReceived);
    QNetworkRequest request(QUrl("https://api.ipify.org/?format=json"));
    manager->get(request);
}

void Client::onPublicIpReceived(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QString publicIp = doc.object()["ip"].toString();
        qDebug() << "Public IP address is:" << publicIp;
        connectToServer(publicIp, 12345);
    } else {
        qWarning() << "Failed to get public IP address:" << reply->errorString();
    }
    reply->deleteLater();
}

// -------------------- Incoming data processing --------------------

void Client::onReadyRead() {
    static QByteArray buffer;
    buffer.append(m_socket->readAll());

    QList<QByteArray> messages = buffer.split('\n\n');
    for (int i = 0; i < messages.size() - 1; ++i) {
        processMessage(messages[i].trimmed());
    }
    buffer = messages.last();
}

void Client::processMessage(const QByteArray &message) {
    if (message.startsWith("ID:")) {
        processIdMessage(QString::fromUtf8(message));
    } else if (message == "START_GAME") {
        emit gameStarted();
    } else if (message == "CLIENT_SHUTDOWN"){
        clientGameManager->server_closed = true;
    }else {
        processJsonMessage(QString::fromUtf8(message));
    }
}

void Client::processIdMessage(const QString &message) {
    bool ok;
    int receivedId = message.mid(3).toInt(&ok);
    if (ok) {
        id = receivedId;
        clientGameManager->setId(id);
        emit idReceived(id);
    } else {
        qWarning() << "Failed to parse ID from server message:" << message;
    }
}

void Client::processJsonMessage(const QString &message) {
    QByteArray utf8Data = message.toUtf8();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(utf8Data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        qWarning() << "Invalid JSON message:" << parseError.errorString();
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    QString type = jsonObject["type"].toString();
    if (type == "GAME_DATA") {
        processGameData(jsonObject);
    } else {
        clientGameManager->processDataFromServer(jsonObject);
        qDebug() << "OVO PROSLEDJUJEM CLIENTGAMEMANAGERU:" << jsonObject;
    }
}

void Client::processGameData(const QJsonObject &jsonObject) {
    if (!jsonObject.contains("gameData")) {
        qWarning() << "GAME_DATA message is missing 'gameData'.";
        return;
    }
    QJsonObject gameData = jsonObject["gameData"].toObject();
    clientGameManager->processLoadData(gameData);
    qDebug() << "Received GAME_DATA from server:" << gameData;
}

// -------------------- Slots for events --------------------

void Client::onGameStarted() {
    qDebug() << "Game started!";
}

void Client::onGameOver(const QString &reason) {
    qDebug() << "Game over: " << reason;
}
