#ifndef USER_H
#define USER_H

#include "server.h"

#include <QVariant>
#include <QSqlQuery>
#include <QUdpSocket>
#include <QDataStream>
#include <QHostAddress>

#pragma pack(1)
typedef struct {
    unsigned short client_port;
    short signal_type;
    char client_address[15];
    char username[10];
    char password[15];
} user_info;
#pragma pack()

class User: public Server {
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
signals:
    void signup_signal(QHostAddress remote, unsigned short port, string username, string passwrod);
    void signin_signal(QHostAddress remote, unsigned short port, string username, string passwrod);
private slots:
    void processPendingDatagram();
    void creatUser(QHostAddress remote, unsigned short port, string username, string passwrod);
    void validateUser(QHostAddress remote, unsigned short port, string username, string passwrod);
};

#endif // USER_H
