#include "server.h"

Server::Server(QObject *parent): QObject(parent) {
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

