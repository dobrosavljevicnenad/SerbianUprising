#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendData(const QString &data);
signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();
private:
    QTcpServer *m_server;
    QTcpSocket *m_clientSocket; // Host
    QTcpSocket *m_secondPlayerSocket; // Second Player
    bool m_gameStarted;
    bool m_waitingForSecondPlayer;  // True if waiting for second player to reconnect
};
#endif // SERVER_H
