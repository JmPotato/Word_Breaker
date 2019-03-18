#include "user.h"

User::User(QObject *parent): Server(parent) {
    connect(this, &User::signupSignal, this, &User::creatUser);
    connect(this, &User::signinSignal, this, &User::validateUser);
    connect(this, &User::rankSignal, this, &User::getRank);
}

void User::processPendingDatagram() {
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
            case 1:
                cout << "CREAT USER REQUEST" << endl;
                emit signupSignal(client_address, client_port, recPacket);
                break;
            case 2:
                cout << "CHECK USER REQUEST" << endl;
                emit signinSignal(client_address, client_port, recPacket);
                break;
            case 3:
                cout << "GET RANK REQUEST" << endl;
                emit rankSignal(client_address, client_port);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void User::creatUser(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString userType;
    QString user_type;
    if(recPacket.userType == 0)
        user_type = "maker";
    else if(recPacket.userType == 1)
        user_type = "breaker";
    QString sql = QString("SELECT username FROM %1 WHERE username=\"%2\";").arg(user_type).arg(QString::fromStdString(recPacket.username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    datagram.resize(sizeof(traPacket));
    traPacket.signalType = -1;
    if(!sql_query.first()) {
        sql = QString("INSERT INTO %1 (username, password, mark, xp, level) VALUES (\"%2\", \"%3\", 0, 0, 0);").arg(user_type).arg(QString::fromStdString(recPacket.username), QString::fromStdString(recPacket.password));
        try {
            QSqlQuery sql_query = database.execute(sql);
            traPacket.signalType = 1;
        } catch(...) {
            traPacket.signalType = -1;
        }
    }
    traPacket.userType = recPacket.userType;
    strcpy(traPacket.password, "");
    strcpy(traPacket.username, "");
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}

void User::validateUser(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString user_type;
    if(recPacket.userType == 0)
        user_type = "maker";
    else if(recPacket.userType == 1)
        user_type = "breaker";
    QString sql = QString("SELECT password FROM %1 WHERE username=\"%2\";").arg(user_type).arg(QString::fromStdString(recPacket.username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    traPacket.signalType = -2;
    if(sql_query.first()) {
        if(!QString::compare(sql_query.value(0).toString(), QString::fromStdString(recPacket.password)))
            traPacket.signalType = 2;
    }
    traPacket.userType = recPacket.userType;
    strcpy(traPacket.password, "");
    strcpy(traPacket.username, "");
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}

void User::getRank(QHostAddress remote, unsigned short port) {
    QByteArray datagram;
    Packet traPacket;
    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
    tableViewModel->setQuery("SELECT (username, mark, xp, level) FROM breaker ORDER BY level");
    traPacket.signalType = 3;
    traPacket.userType = -1;
    strcpy(traPacket.password, "");
    strcpy(traPacket.username, "");
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}
