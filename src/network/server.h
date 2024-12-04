#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "../base/Mechanics/turn.h"
#include "../base/graph/graph.hpp"
#include <vector>
#include "../base/Mechanics/Action.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendData(const QString &data);
    ////////////////////////////////////////////////
    Turn turn;
    std::vector<Action> actionsPlayer1;
    std::vector<Action> actionsPlayer2;
    void executeActions(const std::vector<Action> &actions);
    void broadcast(const QString &message);
////////////////////////////////////////////////
signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
    void playerJoined(int playerId); // Signal emitted when a player joins
private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();
private:
    QTcpServer *m_server = nullptr;
    QTcpSocket *m_clientSocket; // Host
    QTcpSocket *m_secondPlayerSocket; // Second Player
    bool m_gameStarted;
    bool m_waitingForSecondPlayer;  // True if waiting for second player to reconnect
    graph::Graph g;
};
#endif // SERVER_H
