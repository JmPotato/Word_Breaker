#ifndef RANK_H
#define RANK_H


#include "client.h"

#include <iostream>

#include <QHostAddress>

#define GET_USER_BY_RANK  1

class Rank: public Client {
    Q_OBJECT
public:
    QStringList wordList;
    unsigned short difficulty;
    unsigned short currentLevel = 1;
    unsigned short currentTimeLimted = 5;

    explicit Rank(QObject *parent = nullptr);
    void getRank(short type);
    short send(const QByteArray data);
signals:
    void reciveUserSignal(Packet recPacket);
private slots:
    void processPendingDatagram();
};

#endif // RANK_H
