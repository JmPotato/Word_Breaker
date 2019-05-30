#ifndef GAME_H
#define GAME_H

#include "server.h"

#include <QVariant>
#include <QSqlQuery>
#include <QHostAddress>
#include <QSqlQueryModel>

#define UPDATE_WORD_REQUEST 1
#define GET_WROD_REQUEST    2

class Game: public Server {
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
signals:
    void getWordSignal(QHostAddress remote, unsigned short port, Packet recPacket);     //拉取单词请求信号
    void updateWordSignal(QHostAddress remote, unsigned short port, Packet recPacket);  //更新单词请求信号
private slots:
    void processPendingDatagram();      //数据帧处理槽函数
    void getWord(QHostAddress remote, unsigned short port, Packet recPacket);   //拉取单词槽函数
    void updateWord(QHostAddress remote, unsigned short port, Packet recPacket);    //更新单词槽函数
};

#endif // GAME_H
