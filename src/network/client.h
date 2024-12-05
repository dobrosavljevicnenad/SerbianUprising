#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "../base/Mechanics/Action.h"
#include "../Interface/clientgamemanager.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    bool connectToServer(const QString &hostAddress, quint16 port);
    void sendData(const QString &data);
    // void sendAction(const Action &action, int id);
    void sendEndTurnWithActions(const QVector<Action> &actions, int id);
    // void sendEndTurn();
    void disconnectFromServer();
    int getId() const;
    void setId(int newId);
    ClientGameManager* getClientGameManager() const;

signals:
    void dataReceived(const QString &data);
    void gameStarted();
    void gameOver(const QString &reason);
    void playerIdAssigned(int playerId);
    void idReceived(int id);

public slots:
    void onReadyRead();

private slots:
    void onGameStarted();
    void onGameOver(const QString &reason);

private:
    QTcpSocket *m_socket;
    int id;
    ClientGameManager* clientGameManager;
};

#endif // CLIENT_H
