#include "user.h"

User::User(QObject *parent): Server(parent) {
    connect(this, &User::signupSignal, this, &User::creatUser);
    connect(this, &User::signinSignal, this, &User::validateUser);
}

void User::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QHostAddress client_address;
        unsigned short client_port;
        QByteArray datagram;
        user_info user;
        char buf[100];
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);
        QDataStream a(&datagram, QIODevice::ReadOnly);
        a.readRawData(buf, sizeof(user));
        memcpy(&user, buf, sizeof(user));
        cout << client_address.toString().toStdString() << ":" << client_port << endl;
        switch(user.signalType) {
            case 1:
                cout << "CREAT USER REQUEST" << endl;
                emit signupSignal(client_address, client_port, user.userType, user.username, user.password);
                break;
            case 2:
                cout << "CHECK USER REQUEST" << endl;
                emit signinSignal(client_address, client_port, user.userType, user.username, user.password);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void User::creatUser(QHostAddress remote, unsigned short port, short type, string username, string password) {
    QString userType;
    if(type == 0)
        userType = "maker";
    else if(type == 1)
        userType = "breaker";
    QString sql = QString("SELECT username FROM %1 WHERE username=\"%2\";").arg(userType).arg(QString::fromStdString(username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    datagram.resize(2);
    datagram[0] = 0x00;
    datagram[1] = 0x00;
    if(!sql_query.first()) {
        sql = QString("INSERT INTO %1 (username, password, mark, xp, level) VALUES (\"%2\", \"%3\", 0, 0, 0);").arg(userType).arg(QString::fromStdString(username), QString::fromStdString(password));
        try {
            QSqlQuery sql_query = database.execute(sql);
            datagram[0] = 0x01;
        } catch(...) {
            datagram[0] = 0x00;
        }
    }
    socket->writeDatagram(datagram.data(), datagram.size(), remote, port);
}

void User::validateUser(QHostAddress remote, unsigned short port, short type, string username, string passwrod) {
    QString user_type;
    if(type == 0)
        user_type = "maker";
    else if(type == 1)
        user_type = "breaker";
    QString sql = QString("SELECT password FROM %1 WHERE username=\"%2\";").arg(user_type).arg(QString::fromStdString(username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    datagram.resize(2);
    datagram[0] = 0x00;
    datagram[1] = 0x00;
    if(sql_query.first()) {
        if(!QString::compare(sql_query.value(0).toString(), QString::fromStdString(passwrod)))
            datagram[0] = 0x01;
    }
    socket->writeDatagram(datagram.data(), datagram.size(), remote, port);
}
