#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <random>
#include <ctime>
#include <iostream>

#include <QByteArray>
#include <QUdpSocket>
#include <QDataStream>
#include <QHostInfo>
#include <QHostAddress>

using namespace std;

class Client: public QObject {
    Q_OBJECT
    QUdpSocket *socket;
public:
    QHostAddress clientAddress;
    unsigned short clientPort;
    explicit Client(QObject *parent = nullptr);
    int send(const QByteArray data);
signals:
    void successSignal(short status=1);
    void failSignal(short status=0);
public slots:
    void processPendingDatagram();
};

#endif // CLIENT_H
