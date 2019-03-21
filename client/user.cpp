#include "user.h"

User::User(QObject *parent): Client(parent) {
    id = -1;
    status = 0;
    userType = -1;
    username = "";
    password = "";
}

void User::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        Packet recPacket;
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        unpackPacket(datagram, recPacket);
        cout << recPacket.signalType << endl;
        switch(recPacket.signalType) {
            case 1:
            case -1:
                cout << "CREAT USER RESPONSE" << endl;
                emit signupSignal(recPacket);
                break;
            case 2:
            case -2:
                cout << "CHECK USER RESPONSE" << endl;
                emit signinSignal(recPacket);
                break;
            case 3:
            case -3:
                cout << "GET USER RESPONSE" << endl;
                emit getInfoSignal(recPacket);
                break;
            default:
                cout << "FAILED RESPONSE" << endl;
                break;
        }
    }
}

void User::insertUser() {
    QByteArray data;
    Packet user;
    user.signalType = 1;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    packPacket(data, user);
    send(data);
}

void User::checkUser() {
    QByteArray data;
    Packet user;
    user.signalType = 2;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    packPacket(data, user);
    send(data);
}

void User::signinUser() {
    QByteArray data;
    Packet user;
    user.signalType = 3;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    packPacket(data, user);
    send(data);
}

void User::signoutUser() {
    id = -1;
    status = 0;
    userType = -1;
    username = "";
    password = "";
}
