#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "../base/Mechanics/Action.h"
#include "../Interface/clientgamemanager.h"
#include "../clientwindow.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    // Public API
    bool connectToServer(const QString &hostAddress, quint16 port);
    void disconnectFromServer();
    void sendData(const QString &data);
    void getPublicIp();

    // Getters and Setters
    int getId() const;
    void setId(int newId);
    ClientGameManager* getClientGameManager() const;

    // Message Processing
    void processIdMessage(const QString &message);
    void processJsonMessage(const QString &message);
    void processGameData(const QJsonObject &jsonObject);

signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
    void playerIdAssigned(int playerId);
    void idReceived(int id);

public slots:
    void onReadyRead();
    void sendEndTurnWithActions(const QVector<Action> &actions, int id);
    void handleLoadGame(const QJsonObject &graphData);

private slots:
    void onGameStarted();
    void onGameOver(const QString &reason);

private:
    // Helper Methods
    void setupConnections();
    QJsonObject prepareEndTurnMessage(const QVector<Action> &actions, int id) const;
    void processMessage(const QByteArray &message);
    void onPublicIpReceived(QNetworkReply *reply);


    // Member Variables
    QTcpSocket *m_socket;
    int id;
    ClientGameManager* clientGameManager;
};

#endif // CLIENT_H
