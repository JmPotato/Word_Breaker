#ifndef RANK_H
#define RANK_H


#include "client.h"

#include <iostream>

#include <QHostAddress>

#define GET_USER_BY_RANK  1

class Rank: public Client {
    Q_OBJECT
public:
    explicit Rank(QObject *parent = nullptr);   //构造函数
    void getRank(short type);                   //拉取排行榜
    short send(const QByteArray data);          //发送数据帧
signals:
    void reciveUserSignal(Packet recPacket);    //用户数据到达信号
private slots:
    void processPendingDatagram();              //接受处理数据帧（虚函数）
};

#endif // RANK_H
