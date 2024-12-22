#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include "../base/Mechanics/Action.h"
#include "../Interface/clientgamemanager.h"
#include "../clientwindow.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    bool connectToServer(const QString &hostAddress, quint16 port);
    void sendData(const QString &data);
    // void sendAction(const Action &action, int id);
    // void sendEndTurn();
    void disconnectFromServer();
    int getId() const;
    void setId(int newId);
    ClientGameManager* getClientGameManager() const;
    void processIdMessage(const QString &message);
    void processJsonMessage(const QString &message);
    void processGameData(const QJsonObject &jsonObject);

    void getPublicIp();
    void onPublicIpReceived(QNetworkReply *reply);
signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
    void playerIdAssigned(int playerId);
    void idReceived(int id);

public slots:
    void onReadyRead();
    void sendEndTurnWithActions(const QVector<Action> &actions, int id);
    void handleLoadGame(const QJsonObject& graphData);

private slots:
    void onGameStarted();
    void onGameOver(const QString &reason);

private:
    QTcpSocket *m_socket;
    int id;
    ClientGameManager* clientGameManager;
};

#endif // CLIENT_H
