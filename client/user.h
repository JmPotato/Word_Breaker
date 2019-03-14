#ifndef USER_H
#define USER_H

#include "client.h"

#include <iostream>

#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QElapsedTimer>

using namespace std;

#pragma pack(1)
typedef struct {
    short signalType;
    short userType;
    char username[10];
    char password[15];
} user_info;
#pragma pack()

class User: public QObject {
    Q_OBJECT
private:
    Client client;
    short id;
public:
    short status;
    short userType;
    QString username;
    QString password;
    explicit User(QObject *parent = nullptr);
    int checkUser();
    int insertUser();
    void signoutUser();
signals:
    void dataSent();
public slots:
    void changeStatus(short value);
};

#endif // USER_H
