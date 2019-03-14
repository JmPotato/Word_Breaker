#include "user.h"

User::User(QObject *parent): QObject(parent) {
    id = -1;
    status = 0;
    userType = -1;
    username = "";
    password = "";
    connect(this, &User::dataSent, &client, &Client::processPendingDatagram);
    connect(&client, &Client::successSignal, this, &User::changeStatus);
    connect(&client, &Client::failSignal, this, &User::changeStatus);
}

void User::changeStatus(short value) {
    status = value;
}

int User::insertUser() {
    QByteArray data;
    QDataStream w(&data, QIODevice::WriteOnly);
    user_info user;
    char buf[100];
    user.signalType = 1;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    memcpy(buf, &user, sizeof(user));
    w.writeRawData(buf, sizeof(user));
    if(client.send(data)) {
        QElapsedTimer t;
        t.start();
        while(t.elapsed() < 500) {
            t.elapsed();
        }
        emit dataSent();
    }
    return status;
}

int User::checkUser() {
    QByteArray data;
    QDataStream w(&data, QIODevice::WriteOnly);
    user_info user;
    char buf[100];
    user.signalType = 2;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    memcpy(buf, &user, sizeof(user));
    w.writeRawData(buf, sizeof(user));
    if(client.send(data)) {
        QElapsedTimer t;
        t.start();
        while(t.elapsed() < 500) {
            t.elapsed();
        }
        emit dataSent();
    }
    return status;
}

void User::signoutUser() {
    id = -1;
    status = 0;
    userType = -1;
    username = "";
    password = "";
}
