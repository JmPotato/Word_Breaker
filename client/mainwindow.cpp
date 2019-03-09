#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "breaker.h"

#include <fstream>

#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->message->setAlignment(Qt::AlignCenter);
    ui->signup_password->setEchoMode(QLineEdit::Password);
    ui->signup_repassword->setEchoMode(QLineEdit::Password);
    ui->signin_password->setEchoMode(QLineEdit::Password);

    QFileInfo check_db("data.db");
    if(!check_db.exists()) {
        Database sqldata;
        sqldata.initDatabase();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_signupButton_clicked() {
    QString username = ui->signup_username->text();
    QString password = ui->signup_password->text();
    QString repassword = ui->signup_repassword->text();
    if(username != "" && (password == repassword)) {
        Breaker new_breaker(username, password);
        if(new_breaker.insertBreaker())
            ui->stackedWidget->setCurrentIndex(1);
    } else if(username == "")
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不能为空"));
    else if(password != repassword)
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("两次密码输入不一致"));
}

void MainWindow::on_signinButton_clicked() {
    QString username = ui->signin_username->text();
    QString password = ui->signin_password->text();
    Breaker signin_breaker(username, password);
    switch(signin_breaker.checkBreaker()) {
        case 1: ui->stackedWidget->setCurrentIndex(2);break;
        case 0: QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名或密码错误"));break;
        case -1: QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不存在"));break;
    }
}
