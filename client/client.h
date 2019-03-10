#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>

#include <QByteArray>
#include <QUdpSocket>
#include <QDataStream>

using namespace std;

class Client: public QObject {
    Q_OBJECT
    QUdpSocket *socket;
public:
    QByteArray rec_data;
    explicit Client(QObject *parent = nullptr);
    int send(const QByteArray data);
signals:
    void successSignal(short status=1);
    void failSignal(short status=0);
public slots:
    void processPendingDatagram();
};

#endif // CLIENT_H
