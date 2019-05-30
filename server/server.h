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

class Server: public QObject {
    Q_OBJECT
public:
    Database database;  //数据库
    QUdpSocket *socket; //Socket
    explicit Server(QObject *parent = nullptr);
    void packPacket(QByteArray &data, Packet packet);   //打包数据帧
    void unpackPacket(QByteArray data, Packet &packet); //解包数据帧
    short send(const QByteArray data, QHostAddress remote, unsigned short port);    //发送数据帧
private slots:
    virtual void processPendingDatagram() = 0;  //数据帧处虚函数
};

#endif // SERVER_H
