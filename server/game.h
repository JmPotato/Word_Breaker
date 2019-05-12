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
    void getWordSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void updateWordSignal(QHostAddress remote, unsigned short port, Packet recPacket);
private slots:
    void processPendingDatagram();
    void getWord(QHostAddress remote, unsigned short port, Packet recPacket);
    void updateWord(QHostAddress remote, unsigned short port, Packet recPacket);
};

#endif // GAME_H
