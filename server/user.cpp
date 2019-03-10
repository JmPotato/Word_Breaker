#include "user.h"

User::User(QObject *parent): Server(parent) {
    connect(this, SIGNAL(signup_signal(string, string)), this, SLOT(creatUser(string, string)));
    connect(this, SIGNAL(signin_signal(string, string)), this, SLOT(validateUser(string, string)));
}

void User::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        user_info user;
        char buf[50];
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        QDataStream a(&datagram, QIODevice::ReadOnly);
        a.readRawData(buf, sizeof(user));
        memcpy(&user, buf, sizeof(user));
        switch(user.signal_type) {
            case 1:
                cout << "CREAT USER REQUEST" << endl;
                emit signup_signal(user.username, user.password);
                break;
            case 2:
                cout << "CHECK USER REQUEST" << endl;
                emit signin_signal(user.username, user.password);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void User::creatUser(string username, string password) {
    QString sql = QString("SELECT username FROM breaker WHERE username=\"%1\";").arg(QString::fromStdString(username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    datagram.resize(2);
    datagram[0] = 0x00;
    datagram[1] = 0x00;
    if(!sql_query.first()) {
        sql = QString("INSERT INTO breaker (username, password) VALUES (\"%1\", \"%2\");").arg(QString::fromStdString(username), QString::fromStdString(password));
        try {
            QSqlQuery sql_query = database.execute(sql);
            datagram[0] = 0x01;
        } catch(...) {
            datagram[0] = 0x00;
        }
    }
    socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, 4321);
}

void User::validateUser(string username, string passwrod) {
    QString sql = QString("SELECT password FROM breaker WHERE username=\"%1\";").arg(QString::fromStdString(username));
    QSqlQuery sql_query = database.execute(sql);
    QByteArray datagram;
    datagram.resize(2);
    datagram[0] = 0x00;
    datagram[1] = 0x00;
    if(sql_query.first()) {
        if(!QString::compare(sql_query.value(0).toString(), QString::fromStdString(passwrod)))
            datagram[0] = 0x01;
    }
    socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, 4321);
}
