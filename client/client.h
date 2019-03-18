#ifndef CLIENT_H
#define CLIENT_H
#define BUFFER 1024

#include <string>
#include <random>
#include <ctime>
#include <iostream>

#include <QString>
#include <QByteArray>
#include <QUdpSocket>
#include <QDataStream>
#include <QHostInfo>
#include <QHostAddress>

using namespace std;

#pragma pack(1)
typedef struct {
    short signalType;
    short userType;
    char username[10];
    char password[15];
} Packet;
#pragma pack()

class Client: public QObject {
    Q_OBJECT
protected:
    QUdpSocket *socket;
    void packPacket(QByteArray &data, Packet packet);
    void unpackPacket(QByteArray data, Packet &packet);
public:
    QHostAddress clientAddress;
    unsigned short clientPort;
    explicit Client(QObject *parent = nullptr);
    short send(const QByteArray data);
private slots:
    virtual void processPendingDatagram() = 0;
};

#endif // CLIENT_H
