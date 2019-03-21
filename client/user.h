#ifndef USER_H
#define USER_H

#include "client.h"

#include <iostream>

#include <QHostAddress>

using namespace std;

class User: public Client {
    Q_OBJECT
private:
    short id;
public:
    short status;
    short userType;
    QString username;
    QString password;
    explicit User(QObject *parent = nullptr);
    void checkUser();
    void insertUser();
    void signinUser();
    void signoutUser();
signals:
    void signupSignal(Packet recPacket);
    void signinSignal(Packet recPacket);
    void getInfoSignal(Packet recPacket);
private slots:
    void processPendingDatagram();
};

#endif // USER_H
