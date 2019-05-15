#include "rank.h"

Rank::Rank(QObject *parent): Server(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 2345);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    connect(this, &Rank::getUserSignal, this, &Rank::getUserByRank);
}

void Rank::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QHostAddress client_address;
        unsigned short client_port;
        QByteArray datagram;
        Packet recPacket;
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);
        unpackPacket(datagram, recPacket);
        cout << client_address.toString().toStdString() << ":" << client_port << endl;
        switch(recPacket.signalType) {
            case GET_USER_BY_RANK:
                cout << "GET USER BY RANK REQUEST" << endl;
                emit getUserSignal(client_address, client_port, recPacket);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void Rank::getUserByRank(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString user_type;
    if(recPacket.userType == 0)
        user_type = "maker";
    else if(recPacket.userType == 1)
        user_type = "breaker";
    QString sql = QString("SELECT username, mark, xp, level FROM %1 order by xp, mark, level desc limit 10;").arg(user_type);
    QSqlQuery sqlQuery = database.execute(sql);
    if(sqlQuery.first()) {
        QByteArray datagram;
        Packet traPacket;
        strcpy(traPacket.username, sqlQuery.value("username").toString().toStdString().c_str());
        traPacket.mark = sqlQuery.value("mark").toInt();
        traPacket.xp = sqlQuery.value("xp").toInt();
        traPacket.level = sqlQuery.value("level").toInt();
        traPacket.signalType = 1;
        traPacket.userType = recPacket.userType;
        packPacket(datagram, traPacket);
        send(datagram, remote, port);
    } else {
        QByteArray datagram;
        Packet traPacket;
        traPacket.signalType = -1;
        traPacket.userType = recPacket.userType;
        packPacket(datagram, traPacket);
        send(datagram, remote, port);
    }
    while(sqlQuery.next()) {
        QByteArray datagram;
        Packet traPacket;
        strcpy(traPacket.username, sqlQuery.value("username").toString().toStdString().c_str());
        traPacket.mark = sqlQuery.value("mark").toInt();
        traPacket.xp = sqlQuery.value("xp").toInt();
        traPacket.level = sqlQuery.value("level").toInt();
        traPacket.signalType = 1;
        traPacket.userType = recPacket.userType;
        packPacket(datagram, traPacket);
        send(datagram, remote, port);
    }
    QByteArray datagram;
    Packet traPacket;
    traPacket.signalType = -1;
    traPacket.userType = recPacket.userType;
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}
