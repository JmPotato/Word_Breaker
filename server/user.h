#ifndef USER_H
#define USER_H

#include "server.h"

#include <QVariant>
#include <QSqlQuery>
#include <QHostAddress>
#include <QSqlQueryModel>

class User: public Server {
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
signals:
    void signupSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void signinSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void userInfoSignal(QHostAddress remote, unsigned short port, Packet recPacket);
private slots:
    void processPendingDatagram();
    void creatUser(QHostAddress remote, unsigned short port, Packet recPacket);
    void validateUser(QHostAddress remote, unsigned short port, Packet recPacket);
    void getUserInfo(QHostAddress remote, unsigned short port, Packet recPacket);
};

#endif // USER_H
