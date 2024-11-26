#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendData(const QString &data);

signals:
    void dataReceived(const QString &data);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *m_server;
    QTcpSocket *m_clientSocket;
};
