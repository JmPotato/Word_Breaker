#include "breaker.h"

Breaker::Breaker(QObject *parent): QObject(parent) {
    connect(this, &Breaker::dataSent, &client, &Client::processPendingDatagram);
    connect(&client, &Client::successSignal, this, &Breaker::changeStatus);
    connect(&client, &Client::failSignal, this, &Breaker::changeStatus);
}

int Breaker::insertBreaker() {
    QByteArray data;
    QDataStream w(&data, QIODevice::WriteOnly);
    user_info breaker;
    char buf[50];
    breaker.signal_type = 1;
    strcpy(breaker.username, username.toStdString().c_str());
    strcpy(breaker.password, password.toStdString().c_str());
    memcpy(buf, &breaker, sizeof(breaker));
    w.writeRawData(buf, sizeof(breaker));
    if(client.send(data)) {
        QElapsedTimer t;
        t.start();
        while(t.elapsed() < 500) {
            t.elapsed();
        }
        emit dataSent();
    }
    return status;
}

int Breaker::checkBreaker() {
    QByteArray data;
    QDataStream w(&data, QIODevice::WriteOnly);
    user_info breaker;
    char buf[50];
    breaker.signal_type = 2;
    strcpy(breaker.username, username.toStdString().c_str());
    strcpy(breaker.password, password.toStdString().c_str());
    memcpy(buf, &breaker, sizeof(breaker));
    w.writeRawData(buf, sizeof(breaker));
    if(client.send(data)) {
        QElapsedTimer t;
        t.start();
        while(t.elapsed() < 500) {
            t.elapsed();
        }
        emit dataSent();
    }
    return status;
}

void Breaker::changeStatus(short value) {
    status = value;
}
