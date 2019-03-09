#include "breaker.h"
#include "database.h"

#include <iostream>

#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

using namespace std;

Breaker::Breaker(QString name, QString pw) {
    username = name;
    passsword = pw;
}

Breaker::~Breaker() {
    database.close();
}

int Breaker::checkBreaker() {
    QString sql = QString("SELECT password FROM breaker WHERE username=\"%1\";").arg(username);
    QSqlQuery sql_query = database.execute(sql);
    if(sql_query.first()) {
        if(sql_query.value(0).toString() == passsword)
            return 1;
        else
            return 0;
    } else {
        return -1;
    }
}

int Breaker::insertBreaker() {
    QString sql = QString("SELECT username FROM breaker WHERE username=\"%1\";").arg(username);
    QSqlQuery sql_query = database.execute(sql);
    if(sql_query.first()) {
        return 0;
    } else {
        sql = QString("INSERT INTO breaker (username, password) VALUES (\"%1\", \"%2\");").arg(username, passsword);
        try {
            QSqlQuery sql_query = database.execute(sql);
        } catch(...) {
            return -1;
        }
    }
    return 1;
}

void Breaker::getBreakerByID(int id) {
    QString select_breaker_by_id = QString("SELECT * FROM breaker WHERE id=%1;").arg(id);
    QSqlQuery sql_query = database.execute(select_breaker_by_id);
    if(sql_query.first()) {
        breaker_id = id;
        username = sql_query.value(1).toString();
        passsword = sql_query.value(2).toString();
    }
}


void Breaker::getBreakerByName(QString name) {
    QString select_breaker_by_name = QString("SELECT * FROM breaker WHERE username=\"%1\";").arg(name);
    QSqlQuery sql_query = database.execute(select_breaker_by_name);
    if(sql_query.first()) {
        breaker_id = sql_query.value(0).toInt();
        QString pw = sql_query.value(2).toString();
    }
}
