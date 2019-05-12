#include "user.h"

User::User(QObject *parent): Server(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    connect(this, &User::updateSignal, this, &User::updateUser);
    connect(this, &User::signupSignal, this, &User::creatUser);
    connect(this, &User::signinSignal, this, &User::validateUser);
    connect(this, &User::logoutSignal, this, &User::logoutUser);
    connect(this, &User::userInfoSignal, this, &User::getUserInfo);
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
            case CREAT_USER_REQUEST:
                cout << "CREAT USER REQUEST" << endl;
                emit signupSignal(client_address, client_port, recPacket);
                break;
            case CHECK_USER_REQUEST:
                cout << "CHECK USER REQUEST" << endl;
                emit signinSignal(client_address, client_port, recPacket);
                break;
            case GET_USER_REQUEST:
                cout << "GET USER REQUEST" << endl;
                emit userInfoSignal(client_address, client_port, recPacket);
                break;
            case UPDATE_USER_REQUEST:
                cout << "UPDATE USER REQUEST" << endl;
                emit updateSignal(recPacket);
                break;
            case LOGOUT_USER_REQUEST:
                cout << "LOGOUT USER REQUEST" << endl;
                emit logoutSignal(recPacket);
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
    QSqlQuery sqlQuery = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    datagram.resize(sizeof(traPacket));
    traPacket.signalType = -1;
    if(!sqlQuery.first()) {
        sql = QString("INSERT INTO %1 (username, password, mark, xp, level) VALUES (\"%2\", \"%3\", 0, 0, 0);").arg(user_type).arg(QString::fromStdString(recPacket.username), QString::fromStdString(recPacket.password));
        try {
            sqlQuery = database.execute(sql);
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
    QSqlQuery sqlQuery = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    traPacket.signalType = -2;
    if(sqlQuery.first()) {
        if(!QString::compare(sqlQuery.value("password").toString(), QString::fromStdString(recPacket.password))) {
            if(recPacket.userType) {
                if(!breakerOnline.contains(recPacket.username)) {
                    traPacket.signalType = 2;
                    breakerOnline.append(recPacket.username);
                }
            } else {
                if(!makerOnline.contains(recPacket.username)) {
                    traPacket.signalType = 2;
                    makerOnline.append(recPacket.username);
                }
            }
        }
    }
    traPacket.userType = recPacket.userType;
    strcpy(traPacket.password, "");
    strcpy(traPacket.username, "");
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}

void User::getUserInfo(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString user_type;
    if(recPacket.userType == 0)
        user_type = "maker";
    else if(recPacket.userType == 1)
        user_type = "breaker";
    QString sql = QString("SELECT mark, xp, level FROM %1 WHERE username=\"%2\";").arg(user_type).arg(QString::fromStdString(recPacket.username));
    QSqlQuery sqlQuery = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    traPacket.signalType = -3;
    if(sqlQuery.first()) {
        traPacket.mark = sqlQuery.value("mark").toInt();
        traPacket.xp = sqlQuery.value("xp").toInt();
        traPacket.level = sqlQuery.value("level").toInt();
        traPacket.signalType = 3;
    }
    traPacket.userType = recPacket.userType;
    strcpy(traPacket.username, recPacket.username);
    strcpy(traPacket.password, recPacket.password);
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}

void User::updateUser(Packet recPacket) {
    QString user_type;
    if(recPacket.userType == 0)
        user_type = "maker";
    else if(recPacket.userType == 1)
        user_type = "breaker";
    QString sql = QString("UPDATE %1 SET mark=%2, xp=%3, level=%4 WHERE username=\"%5\";").arg(user_type).arg(recPacket.mark).arg(recPacket.xp).arg(recPacket.level).arg(QString::fromStdString(recPacket.username));
    QSqlQuery sqlQuery = database.execute(sql);
}

void User::logoutUser(Packet recPacket) {
    if(recPacket.userType) {
        if(breakerOnline.contains(recPacket.username)) {
            breakerOnline.removeAt(breakerOnline.indexOf(recPacket.username));
        }
    } else {
        if(makerOnline.contains(recPacket.username)) {
            makerOnline.removeAt(makerOnline.indexOf(recPacket.username));
        }
    }
}

//void User::getRank(QHostAddress remote, unsigned short port) {
//    QByteArray datagram;
//    Packet traPacket;
//    QSqlQueryModel *tableViewModel = new QSqlQueryModel;
//    tableViewModel->setQuery("SELECT (username, mark, xp, level) FROM breaker ORDER BY level");
//    traPacket.signalType = 3;
//    traPacket.userType = -1;
//    strcpy(traPacket.password, "");
//    strcpy(traPacket.username, "");
//    packPacket(datagram, traPacket);
//    send(datagram, remote, port);
//}
