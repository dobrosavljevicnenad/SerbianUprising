#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <vector>
#include <QMap>
#include "../Interface/servergamemanager.h"
#include "../base/Mechanics/action.h"

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    // Public Methods
    bool startServer(quint16 port);
    void sendData(const QString &data);
    void broadcast(const QString &message);
    ServerGameManager* getGameManager();
    void executeActions(const std::vector<Action> &actions);

    // Handlers
    void handleLoadGame(const QJsonObject &gameData);
    void handleEndTurn(const QJsonObject &jsonObject);
    void handleLoadGameRequest(const QJsonObject &jsonObject);

    void setPlayerId(int playerId);
signals:
    void startGame();
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);

public slots:
    void onGameStartRequested();
    void handleSerializedGraph_init(const QJsonObject &serializedGraph);
    void handleSerializedGraph(const QJsonObject &serializedGraph, const QJsonObject& results, const QJsonObject& events);
    void handleSerializedGraph_loadGame(QJsonObject& serializedGraph);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    // Private Methods
    void setupPlayerSocket(QTcpSocket* socket, const QString& playerName, const QString& message);
    int firstplayerId = 1;

    // Member Variables
    QTcpServer *m_server = nullptr;
    QTcpSocket *m_clientSocket = nullptr;
    QTcpSocket *m_secondPlayerSocket = nullptr;
    bool m_waitingForSecondPlayer = false;
    ServerGameManager* serverGameManager = nullptr;

    // Gameplay Variables
    int turnCounter = 0;
    QMap<int, QVector<Action>> endTurnActions;
    std::vector<Action> actionsPlayer1;
    std::vector<Action> actionsPlayer2;
};

#endif // SERVER_H
