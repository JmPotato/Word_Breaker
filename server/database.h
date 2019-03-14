#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>

#include <QDir>
#include <QString>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

using namespace std;

class Database {
    QSqlDatabase database;
public:
    Database();
    ~Database();
    void connect();
    void close();
    QSqlQuery execute(QString sqlCommand);
    void initDatabase();
};

#endif // DATABASE_H
