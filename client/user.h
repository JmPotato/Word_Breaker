#ifndef USER_H
#define USER_H

#include "client.h"

#include <iostream>

#include <QHostAddress>

#define CREAT_USER_RESPONSE   1
#define CHECK_USER_RESPONSE   2
#define GET_USER_RESPONSE     3
#define UPGRADE_USER_RESPONSE 4
#define LOGOUT_USER_RESPONSE  5
#define SEARCH_USER_RESPONSE  6

using namespace std;

class User: public Client {
    Q_OBJECT
public:
    short userType;     //用户类型
    QString username;   //用户名
    QString password;   //用户密码
    unsigned short mark;    //单词数
    unsigned short xp;      //经验值
    unsigned short level;   //闯关数

    explicit User(QObject *parent = nullptr);   //构造函数
    void checkUser();       //验证用户
    void insertUser();      //创建用户
    void updateUser();      //更新用户信息
    void searchUser(QString name, short type);      //搜索用户
    void signinUser();      //登陆用户
    void signoutUser();     //注销用户
    short send(const QByteArray data);      //发送数据包
signals:
    void signupSignal(Packet recPacket);    //注册信号
    void signinSignal(Packet recPacket);    //登陆信号
    void getInfoSignal(Packet recPacket);   //用户信息到达信号
    void getResultSignal(Packet recPacket);     //查询数据到达信号
private slots:
    void processPendingDatagram();      //数据帧处理槽函数
};

#endif // USER_H
