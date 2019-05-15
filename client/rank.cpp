#include "rank.h"

Rank::Rank(QObject *parent): Client(parent) {
    socket = new QUdpSocket(this);
    socket->bind(clientAddress, clientPort+2);

    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

short Rank::send(const QByteArray data) {
    try {
        socket->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, 2345);
        return 1;
    } catch (...) {
        return -1;
    }
}

void Rank::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        Packet recPacket;
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        unpackPacket(datagram, recPacket);
        cout << recPacket.signalType << endl;
        switch(recPacket.signalType) {
            case GET_USER_BY_RANK:
            case -GET_USER_BY_RANK:
                cout << "GET USER BY RANK RESPONSE" << endl;
                emit reciveUserSignal(recPacket);
                break;
            default:
                cout << "FAILED RESPONSE" << endl;
                break;
        }
    }
}

void Rank::getRank(short type) {
    QByteArray data;
    Packet rank;
    rank.signalType = 1;
    rank.userType = type;
    packPacket(data, rank);
    send(data);
}
