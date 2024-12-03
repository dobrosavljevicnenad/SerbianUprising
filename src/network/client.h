#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "../base/Mechanics/Action.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    bool connectToServer(const QString &hostAddress, quint16 port);
    void sendData(const QString &data);
    void sendAction(const Action &action);
    void sendEndTurn();

signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
    void playerIdAssigned(int playerId);

private slots:
    void onReadyRead();
    void onGameStarted();
    void onGameOver(const QString &reason);

private:
    QTcpSocket *m_socket;
};

#endif // CLIENT_H
