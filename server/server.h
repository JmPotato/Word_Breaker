#ifndef SERVER_H
#define SERVER_H

#include "database.h"

#include <iostream>

#include <QString>
#include <QByteArray>
#include <QUdpSocket>

using namespace std;

class Server: public QObject {
    Q_OBJECT
protected:
    Database database;
    QUdpSocket *socket;
public:
    explicit Server(QObject *parent = nullptr);
private slots:
    virtual void processPendingDatagram() = 0;
};

#endif // SERVER_H
