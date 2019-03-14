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
}

MainWindow::~MainWindow() {
    delete ui;
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
        if(user.insertUser()) {
            ui->stackedWidget->setCurrentIndex(2);
            ui->message->setText(QString("Hello, %1!    Status: %2    Type: %3").arg(user.username).arg(user.status).arg(user.userType));
        } else {
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注册失败：用户名已存在"));
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
    switch(user.checkUser()) {
        case 1:
            ui->stackedWidget->setCurrentIndex(2);
            ui->message->setText(QString("Hello, %1!    \
                                          Status: %2    \
                                          Type: %3").arg(user.username).arg(user.status).arg(user.userType));
            break;
        case 0:
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名或密码错误"));
            user.signoutUser();
            break;
        case -1:
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不存在"));
            user.signoutUser();
            break;
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
    finder = new FinderWindow(this);
    finder->show();
}

void MainWindow::on_rankButton_clicked() {
    rank = new RankWindow(this);
    rank->show();
}
