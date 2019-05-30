#ifndef USER_H
#define USER_H

#include "server.h"

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QHostAddress>
#include <QSqlQueryModel>

#define CREAT_USER_REQUEST   1
#define CHECK_USER_REQUEST   2
#define GET_USER_REQUEST     3
#define UPDATE_USER_REQUEST  4
#define LOGOUT_USER_REQUEST  5
#define SEARCH_USER_REQUEST  6

class User: public Server {
    Q_OBJECT
private:
    QStringList makerOnline;
    QStringList breakerOnline;
public:
    explicit User(QObject *parent = nullptr);
signals:
    void updateSignal(Packet recPacket);    //更新用户信息请求信号
    void signupSignal(QHostAddress remote, unsigned short port, Packet recPacket);  //注册请求信号
    void signinSignal(QHostAddress remote, unsigned short port, Packet recPacket);  //登陆请求信号
    void logoutSignal(Packet recPacket);    //登出请求信号
    void userInfoSignal(QHostAddress remote, unsigned short port, Packet recPacket);    //拉取用户信息请求信号
    void searchUserSignal(QHostAddress remote, unsigned short port, Packet recPacket);  //搜索用户请求信号
private slots:
    void processPendingDatagram();  //数据帧处理槽函数
    void updateUser(Packet recPacket);  //更新用户槽函数
    void logoutUser(Packet recPacket);  //登出用户槽函数
    void creatUser(QHostAddress remote, unsigned short port, Packet recPacket); //创建用户槽函数
    void validateUser(QHostAddress remote, unsigned short port, Packet recPacket);  //验证用户槽函数
    void getUserInfo(QHostAddress remote, unsigned short port, Packet recPacket);   //获取用户信息槽函数
    void searchUserInfo(QHostAddress remote, unsigned short port, Packet recPacket);    //搜索用户信息槽函数
};

#endif // USER_H
