#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    bool connectToServer(const QString &hostAddress, quint16 port);
    void sendData(const QString &data);

signals:
    void dataReceived(const QString &data);

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_socket;
};

#endif // CLIENT_H
