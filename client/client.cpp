#include "client.h"

Client::Client(QObject *parent): QObject(parent) {
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    client_address = info.addresses().first();
    default_random_engine e(time(NULL));
    uniform_int_distribution<unsigned short> u(49152, 65535);
    client_port = u(e);
    cout << client_port << endl;
    socket = new QUdpSocket(this);
    socket->bind(client_address, client_port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

int Client::send(QByteArray data) {
    try {
        socket->writeDatagram(data.data(), data.size(), QHostAddress::LocalHost, 1234);
        return 1;
    } catch (...) {
        return 0;
    }
}

void Client::processPendingDatagram() {
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        char buf[100];
        short res_type = 0;
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        QDataStream a(&datagram, QIODevice::ReadOnly);
        a.readRawData(buf, sizeof(short));
        memcpy(&res_type, buf, sizeof(short));
        switch(res_type) {
            case 1:
                cout << "RES: SUCCESSED" << endl;
                emit successSignal();
                break;
            default:
                cout << "RES: FAILED" << endl;
                emit failSignal();
                break;
        }
    }
}

