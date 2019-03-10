#ifndef USER_H
#define USER_H

#include "server.h"

#include <QVariant>
#include <QSqlQuery>
#include <QUdpSocket>
#include <QDataStream>

#pragma pack(1)
typedef struct {
    short signal_type;
    char username[10];
    char password[15];
} user_info;
#pragma pack()

class User: public Server {
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
signals:
    void signup_signal(string username, string passwrod);
    void signin_signal(string username, string passwrod);
private slots:
    void processPendingDatagram();
    void creatUser(string username, string passwrod);
    void validateUser(string username, string passwrod);
};

#endif // USER_H
