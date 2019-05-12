#ifndef USER_H
#define USER_H

#include "client.h"

#include <iostream>

#include <QHostAddress>

#define CREAT_USER_RESPONSE   1
#define CHECK_USER_RESPONSE   2
#define GET_USER_RESPONSE     3
#define UPGRADE_USER_RESPONSE 4

using namespace std;

class User: public Client {
    Q_OBJECT
public:
    short userType;
    QString username;
    QString password;
    unsigned short mark;
    unsigned short xp;
    unsigned short level;

    explicit User(QObject *parent = nullptr);
    void checkUser();
    void insertUser();
    void updateUser();
    void signinUser();
    void signoutUser();
    short send(const QByteArray data);
signals:
    void signupSignal(Packet recPacket);
    void signinSignal(Packet recPacket);
    void getInfoSignal(Packet recPacket);
private slots:
    void processPendingDatagram();
};

#endif // USER_H
