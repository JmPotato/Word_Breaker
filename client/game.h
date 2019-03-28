#ifndef GAME_H
#define GAME_H

#include "client.h"

#include <iostream>

#include <QHostAddress>

class Game: public Client {
    Q_OBJECT
public:
    QStringList wordList;
    explicit Game(QObject *parent = nullptr);
    void getWord(unsigned short wordLength);
    void updateWord(string _word);
    short send(const QByteArray data);
signals:
    void getWordSignal(Packet recPacket);
    void updateWordSignal(Packet recPacket);
private slots:
    void processPendingDatagram();
};

#endif // GAME_H
