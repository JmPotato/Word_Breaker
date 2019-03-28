#include "server.h"

Server::Server(QObject *parent): QObject(parent) {
}

void Server::packPacket(QByteArray &data, Packet packet) {
    char buf[BUFFER];
    QDataStream w(&data, QIODevice::WriteOnly);
    memcpy(buf, &packet, sizeof(packet));
    w.writeRawData(buf, sizeof(packet));
}

void Server::unpackPacket(QByteArray data, Packet &packet) {
    char buf[BUFFER];
    QDataStream r(&data, QIODevice::ReadOnly);
    r.readRawData(buf, sizeof(packet));
    memcpy(&packet, buf, sizeof(packet));
}

short Server::send(const QByteArray data, QHostAddress remote, unsigned short port) {
    try {
        socket->writeDatagram(data.data(), data.size(), remote, port);
        return 1;
    } catch (...) {
        return 0;
    }
}
