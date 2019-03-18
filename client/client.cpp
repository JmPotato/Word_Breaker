#include "client.h"

Client::Client(QObject *parent): QObject(parent) {
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    clientAddress = info.addresses().first();
    default_random_engine e(time(NULL));
    uniform_int_distribution<unsigned short> u(49152, 65535);
    clientPort = u(e);
    cout << clientPort << endl;
    socket = new QUdpSocket(this);
    socket->bind(clientAddress, clientPort);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

void Client::packPacket(QByteArray &data, Packet packet) {
    char buf[BUFFER];
    QDataStream w(&data, QIODevice::WriteOnly);
    memcpy(buf, &packet, sizeof(packet));
    w.writeRawData(buf, sizeof(packet));
}

void Client::unpackPacket(QByteArray data, Packet &packet) {
    char buf[BUFFER];
    QDataStream r(&data, QIODevice::ReadOnly);
    r.readRawData(buf, sizeof(packet));
    memcpy(&packet, buf, sizeof(packet));
}

short Client::send(const QByteArray data) {
    try {
        socket->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, 1234);
        return 1;
    } catch (...) {
        return 0;
    }
}

