#include "client.h"
#include <QDebug>

// -------------------- Konstruktor --------------------

Client::Client(QObject *parent)
    : QObject(parent),
    m_socket(new QTcpSocket(this)),
    clientGameManager(new ClientGameManager(nullptr, this))
{
    // Povezivanje signala i slotova
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // readyRead i onReadyRead, kada stignu podaci
    // sa servera automatski se povezuje sa metodom onReadyRead()
    connect(clientGameManager, &ClientGameManager::endTurnActionsReady, this, &Client::sendEndTurnWithActions);
    connect(clientGameManager, &ClientGameManager::gameDataLoaded, this, &Client::handleLoadGame);
}

// -------------------- Getteri i setteri --------------------

ClientGameManager* Client::getClientGameManager() const {
    return clientGameManager;
}

void Client::setId(int newId) {
    id = newId;
}

int Client::getId() const {
    return id;
}

// -------------------- Povezivanje sa serverom --------------------

bool Client::connectToServer(const QString &hostAddress, quint16 port)
{
    m_socket->connectToHost(hostAddress, port);
    if (!m_socket->waitForConnected(5000)) {
        qWarning() << "Could not connect to server.";
        return false;
    }
    return true;
}

void Client::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(3000); // Čekanje na diskonektovanje
        }
    }
}

// -------------------- Slanje podataka --------------------

void Client::sendData(const QString &data)
{
    QByteArray jsonData = data.toUtf8();
    QByteArray header = QByteArray::number(jsonData.size()) + "\n"; // Dužina poruke u bajtovima
    m_socket->write(header + jsonData); // Prvo šalje zaglavlje, pa sadržaj
    m_socket->flush();
}

void Client::sendEndTurnWithActions(const QVector<Action> &actions, int id)
{
    QJsonArray actionsArray;
    for (const Action &action : actions) {
        actionsArray.append(QJsonDocument::fromJson(action.toJson().toUtf8()).object());
    }

    QJsonObject jsonObject;
    jsonObject["type"] = "END_TURN";
    jsonObject["id"] = id;
    jsonObject["actions"] = actionsArray;

    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact); // Bez duple serializacije
    qDebug() << "Sending JSON to server:" << jsonString;
    m_socket->write(jsonString.toUtf8());
}

void Client::getPublicIp()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Client::onPublicIpReceived);

    // Koristi API za dobijanje javne IP adrese
    QNetworkRequest request(QUrl("https://api.ipify.org/?format=json"));
    manager->get(request);  // Šaljemo GET zahtev
}

void Client::onPublicIpReceived(QNetworkReply *reply)
{
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

// -------------------- Obrada dolaznih podataka --------------------

void Client::onReadyRead()
{
    static QByteArray buffer; // Bafer za pristigle podatke
    buffer.append(m_socket->readAll()); // Dodaj nove podatke u bafer

    // Podeli poruke po separatoru "\n\n"
    QList<QByteArray> messages = buffer.split('\n\n');

    // Obradi sve kompletne poruke, osim poslednje (možda nije završena)
    for (int i = 0; i < messages.size() - 1; ++i) {
        QByteArray message = messages[i].trimmed();

        // Obrada ID poruke
        if (message.startsWith("ID:")) {
            processIdMessage(QString::fromUtf8(message));
        }
        // Obrada START_GAME poruke
        else if (message == "START_GAME") {
            emit gameStarted();
        }
        // Obrada JSON poruke
        else {
            processJsonMessage(QString::fromUtf8(message));
        }
    }

    // Poslednji deo ostaje u baferu ako nije kompletan
    buffer = messages.last();
}

void Client::processIdMessage(const QString &message){
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

void Client::processJsonMessage(const QString &message)
{
    qDebug() << "Processing message:" << message;

    QByteArray utf8Data = message.toUtf8();

    // Provera UTF-8 podudarnosti (opciono)
    if (QString::fromUtf8(utf8Data) != message) {
        qWarning() << "Message contains non-UTF-8 characters.";
        return;
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(utf8Data, &parseError);

    // Provera validnosti JSON-a
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON data from server:"
                   << parseError.errorString() << "at offset"
                   << parseError.offset;
        qDebug() << "Raw data causing error:" << message;
        return;
    }

    // Provera da li je JSON objekat
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "JSON document is not an object:" << message;
        return;
    }

    // Procesiranje JSON objekta
    QJsonObject jsonObject = jsonDoc.object();
    QString type = jsonObject["type"].toString();

    qDebug() << "Message type:" << type;
    if (type == "GAME_DATA") {
        qDebug() << "Processing GAME_DATA...";
        processGameData(jsonObject);
    } else {
        qDebug() << "Processing other data for clientGameManager...";
        clientGameManager->processDataFromServer(jsonObject);
    }
}


void Client::processGameData(const QJsonObject &jsonObject)
{
    if (!jsonObject.contains("gameData")) {
        qWarning() << "GAME_DATA message is missing 'gameData'.";
        return;
    }

    QJsonObject gameData = jsonObject["gameData"].toObject();
    clientGameManager->processLoadData(gameData);
    qDebug() << "Received GAME_DATA from server:" << gameData;
}

// -------------------- Slotovi za događaje --------------------

void Client::onGameStarted()
{
    qDebug() << "Game started!";
}

void Client::onGameOver(const QString &reason)
{
    qDebug() << "Game over: " << reason;
}
