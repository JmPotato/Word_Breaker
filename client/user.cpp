#include "user.h"

User::User(QObject *parent): Client(parent) {
    socket = new QUdpSocket(this);
    socket->bind(clientAddress, clientPort);

    userType = -1;
    username = "";
    password = "";
    mark = 0;
    xp = 0;
    level = 0;

    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

short User::send(const QByteArray data) {
    try {
        socket->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, 1234);
        return 1;
    } catch (...) {
        return -1;
    }
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
            case CREAT_USER_RESPONSE:
            case -CREAT_USER_RESPONSE:
                cout << "CREAT USER RESPONSE" << endl;
                emit signupSignal(recPacket);
                break;
            case CHECK_USER_RESPONSE:
            case -CHECK_USER_RESPONSE:
                cout << "CHECK USER RESPONSE" << endl;
                emit signinSignal(recPacket);
                break;
            case GET_USER_RESPONSE:
            case -GET_USER_RESPONSE:
                cout << "GET USER RESPONSE" << endl;
                emit getInfoSignal(recPacket);
                break;
            case UPGRADE_USER_RESPONSE:
            case -UPGRADE_USER_RESPONSE:
                cout << "UPGRADE USER RESPONSE" << endl;
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


void User::updateUser() {
    QByteArray data;
    Packet user;
    user.signalType = 4;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    strcpy(user.password, password.toStdString().c_str());
    user.mark = mark;
    user.xp = xp;
    user.level = level;
    packPacket(data, user);
    send(data);
}

void User::signoutUser() {
    QByteArray data;
    Packet user;
    user.signalType = 5;
    user.userType = userType;
    strcpy(user.username, username.toStdString().c_str());
    packPacket(data, user);
    send(data);
    userType = -1;
    username = "";
    password = "";
    xp = 0;
    mark = 0;
    level = 0;
}
