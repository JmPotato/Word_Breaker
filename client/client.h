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
    short signalType;       //信号类型，用于判断本次传输要进行的操作
    short userType;         //用户类型
    unsigned short mark;    //单词数
    unsigned short xp;      //经验值
    unsigned short level;   //闯关数
    unsigned short length;  //单词长度
    char username[10];      //用户名
    char password[15];      //用户密码
    char word[20];          //单词
} Packet;
#pragma pack()

class Client: public QObject {
    Q_OBJECT
public:
    QUdpSocket *socket;
    unsigned short clientPort;
    QHostAddress clientAddress;
    explicit Client(QObject *parent = nullptr);
    void packPacket(QByteArray &data, Packet packet);
    void unpackPacket(QByteArray data, Packet &packet);
    virtual short send(const QByteArray data) = 0;
private slots:
    virtual void processPendingDatagram() = 0;
};

#endif // CLIENT_H
