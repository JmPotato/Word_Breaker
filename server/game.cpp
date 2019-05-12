#include "game.h"

Game::Game(QObject *parent): Server(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 4321);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    connect(this, &Game::updateWordSignal, this, &Game::updateWord);
    connect(this, &Game::getWordSignal, this, &Game::getWord);
}

void Game::processPendingDatagram() {
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
            case UPDATE_WORD_REQUEST:
                cout << "UPDATE WORD REQUEST" << endl;
                emit updateWordSignal(client_address, client_port, recPacket);
                break;
            case GET_WROD_REQUEST:
                cout << "GET WORD REQUEST" << endl;
                emit getWordSignal(client_address, client_port, recPacket);
                break;
            default:
                cout << "NONE REQUEST" << endl;
                break;
        }
    }
}

void Game::updateWord(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString sql = QString("SELECT word FROM word_table WHERE word=\"%1\";").arg(QString::fromStdString(recPacket.word));
    QSqlQuery sqlQuery = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    datagram.resize(sizeof(traPacket));
    traPacket.signalType = -1;
    if(!sqlQuery.first()) {
        sql = QString("INSERT INTO word_table (word, length) VALUES (\"%1\", %2);").arg(QString::fromStdString(recPacket.word)).arg(QString::fromStdString(recPacket.word).length());
        try {
            sqlQuery = database.execute(sql);
            traPacket.signalType = 1;
        } catch(...) {
            traPacket.signalType = -1;
        }
    }
    strcpy(traPacket.word, recPacket.word);
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}

void Game::getWord(QHostAddress remote, unsigned short port, Packet recPacket) {
    QString sql = QString("SELECT word FROM word_table WHERE length=%1 order by random() limit 1;").arg(recPacket.length);
    QSqlQuery sqlQuery = database.execute(sql);
    QByteArray datagram;
    Packet traPacket;
    datagram.resize(sizeof(traPacket));
    traPacket.signalType = -2;
    if(sqlQuery.first()) {
        traPacket.signalType = 2;
        strcpy(traPacket.word, sqlQuery.value("word").toString().toStdString().c_str());
    }
    traPacket.userType = recPacket.userType;
    packPacket(datagram, traPacket);
    send(datagram, remote, port);
}
