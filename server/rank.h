#ifndef RANK_H
#define RANK_H

#include "server.h"

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QHostAddress>
#include <QSqlQueryModel>

#define GET_USER_BY_RANK  1

class Rank: public Server {
    Q_OBJECT
public:
    explicit Rank(QObject *parent = nullptr);
signals:
    void getUserSignal(QHostAddress remote, unsigned short port, Packet recPacket);     //排行榜数据请求信号
private slots:
    void processPendingDatagram();      //数据帧处理槽函数
    void getUserByRank(QHostAddress remote, unsigned short port, Packet recPacket);     //排行榜数据拉取槽函数
};

#endif // RANK_H
