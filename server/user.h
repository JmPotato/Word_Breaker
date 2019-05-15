#ifndef USER_H
#define USER_H

#include "server.h"

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QHostAddress>
#include <QSqlQueryModel>

#define CREAT_USER_REQUEST   1
#define CHECK_USER_REQUEST   2
#define GET_USER_REQUEST     3
#define UPDATE_USER_REQUEST  4
#define LOGOUT_USER_REQUEST  5
#define SEARCH_USER_REQUEST  6

class User: public Server {
    Q_OBJECT
private:
    QStringList makerOnline;
    QStringList breakerOnline;
public:
    explicit User(QObject *parent = nullptr);
signals:
    void updateSignal(Packet recPacket);
    void signupSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void signinSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void logoutSignal(Packet recPacket);
    void userInfoSignal(QHostAddress remote, unsigned short port, Packet recPacket);
    void searchUserSignal(QHostAddress remote, unsigned short port, Packet recPacket);
private slots:
    void processPendingDatagram();
    void updateUser(Packet recPacket);
    void logoutUser(Packet recPacket);
    void creatUser(QHostAddress remote, unsigned short port, Packet recPacket);
    void validateUser(QHostAddress remote, unsigned short port, Packet recPacket);
    void getUserInfo(QHostAddress remote, unsigned short port, Packet recPacket);
    void searchUserInfo(QHostAddress remote, unsigned short port, Packet recPacket);
};

#endif // USER_H
