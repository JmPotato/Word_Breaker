#ifndef BREAKER_H
#define BREAKER_H

#include "client.h"

#include <iostream>

#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QElapsedTimer>

using namespace std;

#pragma pack(1)
typedef struct {
    unsigned short client_port;
    short signal_type;
    char client_address[15];
    char username[10];
    char password[15];
} user_info;
#pragma pack()

class Breaker: public QObject {
    Q_OBJECT
private:
    Client client;
public:
    short status;
    QString username;
    QString password;
    explicit Breaker(QObject *parent = nullptr);
    int checkBreaker();
    int insertBreaker();
signals:
    void dataSent();
public slots:
    void changeStatus(short value);
};

#endif // BREAKER_H
