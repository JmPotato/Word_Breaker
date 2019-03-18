#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    ui->signinBreakerButton->toggle();
    ui->signupBreakerButton->toggle();
    ui->signupPassword->setEchoMode(QLineEdit::Password);
    ui->signupRepassword->setEchoMode(QLineEdit::Password);
    ui->signinPassword->setEchoMode(QLineEdit::Password);
    ui->message->setAlignment(Qt::AlignCenter);

    connect(&user, &User::signupSignal, this, &MainWindow::signup);
    connect(&user, &User::signinSignal, this, &MainWindow::signin);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signup(Packet recPacket) {
    if(recPacket.signalType == 1)
        ui->stackedWidget->setCurrentIndex(2);
    else if(recPacket.signalType == -1) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注册失败：该用户名已存在"));
        user.signoutUser();
    }
}

void MainWindow::signin(Packet recPacket) {
    if(recPacket.signalType == 2)
        ui->stackedWidget->setCurrentIndex(2);
    else if(recPacket.signalType == -2) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登陆失败：用户名与密码不匹配"));
        user.signoutUser();
    }
}

void MainWindow::on_signupButton_clicked() {
    QString username = ui->signupUsername->text();
    QString password = ui->signupPassword->text();
    QString repassword = ui->signupRepassword->text();
    if(ui->signupBreakerButton->isChecked())
        user.userType = 1;
    else
        user.userType = 0;
    if(username != "" && (password == repassword)) {
        user.username = username;
        user.password = password;
        if(!user.insertUser()) {
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注册失败：无法连接到服务器"));
            user.signoutUser();
        }
    } else if(username == "") {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不能为空"));
        user.signoutUser();
    } else if(password != repassword) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("两次密码输入不一致"));
        user.signoutUser();
    }
}

void MainWindow::on_signinButton_clicked() {
    QString username = ui->signinUsername->text();
    QString password = ui->signinPassword->text();
    if(ui->signinBreakerButton->isChecked())
        user.userType = 1;
    else
        user.userType = 0;
    user.username = username;
    user.password = password;
    if(!user.checkUser()) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登陆失败：无法连接到服务器"));
        user.signoutUser();
    }
}

void MainWindow::on_gosigninButton_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_gosignupButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_signoutButton_clicked() {
    user.signoutUser();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_findButton_clicked() {
}

void MainWindow::on_rankButton_clicked() {
    rank = new RankWindow(this);
    rank->show();
}
