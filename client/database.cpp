#include "database.h"
#include "breaker.h"

#include <iostream>

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

using namespace std;

Database::Database() {
    connect();
}

Database::~Database() {
    close();
}

void Database::connect() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("data.db");
    }
    if (!database.open()) {
        cout << "Error: Failed to connect database." << endl;
    }
}

void Database::close() {
    database.close();
}

QSqlQuery Database::execute(QString sql_command) {
    QSqlQuery sql_query;
    if(!sql_query.exec(sql_command)) {
        cout << QString("Error: Failed to execute: %1").arg(sql_command).toStdString() << endl;
        cout << sql_query.lastError().text().toStdString() << endl;
        throw sql_query.lastError();
    }
    return sql_query;
}

void Database::initDatabase() {
    QString create_breaker = "CREATE TABLE breaker ("\
        "breaker_id integer NOT NULL PRIMARY KEY autoincrement,"\
        "username text NOT NULL,"\
        "password text NOT NULL);";
    this->execute(create_breaker);
    QString creat_maker = "CREATE TABLE maker ("\
        "breaker_id integer NOT NULL PRIMARY KEY autoincrement,"\
        "username text NOT NULL,"\
        "password text NOT NULL);";
    this->execute(creat_maker);
}
