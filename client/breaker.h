#ifndef BREAKER_H
#define BREAKER_H

#include "database.h"

#include <QString>

using namespace std;

class Breaker {
private:
    Database database;
    int breaker_id;
    QString username;
    QString passsword;
public:
    Breaker(QString name, QString pw);
    ~Breaker();
    int checkBreaker();
    int insertBreaker();
    void getBreakerByID(int id);
    void getBreakerByName(QString name);
};

#endif // BREAKER_H
