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
    QSqlDatabase database;      //数据库对象
public:
    Database();                 //构造函数
    ~Database();                //析构函数
    void connect();             //连接数据库
    void close();               //关闭数据库
    QSqlQuery execute(QString sqlCommand);      //执行 sql 语句
    void initDatabase();        //创建数据库
};

#endif // DATABASE_H
