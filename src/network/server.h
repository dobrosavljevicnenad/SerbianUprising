#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "../Interface/servergamemanager.h"
#include <vector>
#include "../base/Mechanics/Action.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendData(const QString &data);
    void broadcast(const QString &message);
    ServerGameManager* getGameManager();
    ////////////////////////////////////////////////
    std::vector<Action> actionsPlayer1;
    std::vector<Action> actionsPlayer2;
    void executeActions(const std::vector<Action> &actions);
////////////////////////////////////////////////
signals:
    void startGame();
    ///////////////////////////
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

public slots:
    void onGameStartRequested();
    void handleSerializedGraph(const QJsonObject &serializedGraph);

private:
    QTcpServer *m_server = nullptr;
    QTcpSocket *m_clientSocket; // Host
    QTcpSocket *m_secondPlayerSocket; // Second Player
    bool m_waitingForSecondPlayer;  // True if waiting for second player to reconnect
    ServerGameManager* serverGameManager; // Dedicated game manager
    int turnCounter = 0;
    QMap<int, QVector<Action>> endTurnActions; // cuva akcije po id-u klienta
};
#endif // SERVER_H
