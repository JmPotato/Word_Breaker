#ifndef SERVER_H
#define SERVER_H
#define BUFFER 1024

#include "database.h"

#include <iostream>

#include <QString>
#include <QByteArray>
#include <QUdpSocket>
#include <QDataStream>

using namespace std;

#pragma pack(1)
typedef struct {
    short signalType;
    short userType;
    unsigned short mark;
    unsigned short xp;
    unsigned short level;
    unsigned short length;
    char username[10];
    char password[15];
    char word[20];
} Packet;
#pragma pack()

class Server: public QObject {
    Q_OBJECT
protected:
    Database database;
    QUdpSocket *socket;
public:
    explicit Server(QObject *parent = nullptr);
    void packPacket(QByteArray &data, Packet packet);
    void unpackPacket(QByteArray data, Packet &packet);
    short send(const QByteArray data, QHostAddress remote, unsigned short port);
private slots:
    virtual void processPendingDatagram() = 0;
};

#endif // SERVER_H
