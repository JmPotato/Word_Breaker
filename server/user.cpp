#include "user.h"

User::User(QObject *parent): Server(parent) {
    connect(this, &User::signup_signal, this, &User::creatUser);
    connect(this, &User::signin_signal, this, &User::validateUser);
}

void User::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QHostAddress client;
        QByteArray datagram;
        user_info user;
        char buf[100];
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        QDataStream a(&datagram, QIODevice::ReadOnly);
        a.readRawData(buf, sizeof(user));
        memcpy(&user, buf, sizeof(user));
        client.setAddress(user.client_address);
        cout << user.client_address << ":" << user.client_port << endl;
        switch(user.signal_type) {
            case 1:
                cout << "CREAT USER REQUEST" << endl;
                emit signup_signal(client, user.client_port, user.username, user.password);
                break;
            case 2:
                cout << "CHECK USER REQUEST" << endl;
                emit signin_signal(client, user.client_port, user.username, user.password);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void User::creatUser(QHostAddress remote, unsigned short port, string username, string password) {
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
    socket->writeDatagram(datagram.data(), datagram.size(), remote, port);
}

void User::validateUser(QHostAddress remote, unsigned short port, string username, string passwrod) {
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
    socket->writeDatagram(datagram.data(), datagram.size(), remote, port);
}
