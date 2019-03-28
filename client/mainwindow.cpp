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
    connect(&user, &User::getInfoSignal, this, &MainWindow::getInfo);
    connect(&game, &Game::getWordSignal, this, &MainWindow::showWord);
    connect(&game, &Game::updateWordSignal, this, &MainWindow::updateWordList);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signup(Packet recPacket) {
    if(recPacket.signalType == 1) {
        ui->signupUsername->clear();
        ui->signupPassword->clear();
        ui->signupRepassword->clear();
        ui->stackedWidget->setCurrentIndex(2);
    } else if(recPacket.signalType == -1) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注册失败：该用户名已存在"));
        user.signoutUser();
    }
}

void MainWindow::signin(Packet recPacket) {
    if(recPacket.signalType == 2) {
        ui->signinUsername->clear();
        ui->signinPassword->clear();
        user.signinUser();
        ui->stackedWidget->setCurrentIndex(2);
    } else if(recPacket.signalType == -2) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登陆失败：用户名与密码不匹配"));
        user.signoutUser();
    }
}

void MainWindow::getInfo(Packet recPacket) {
    if(recPacket.signalType == 3) {
        ui->message->setText(QString("Username: %1 Mark: %2 Xp: %3 Level: %4").arg(recPacket.username).arg(recPacket.mark).arg(recPacket.xp).arg(recPacket.level));
    } else if(recPacket.signalType == -3) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("获取用户信息失败"));
        user.signoutUser();
    }
}

void MainWindow::updateWordList(Packet recPacket) {
    if(recPacket.signalType == 1) {
        ui->wordList->addItem(QString::fromStdString(recPacket.word));
        ui->makerWordEdit->clear();
    } else if(recPacket.signalType == -1) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("单词已存在"));
        ui->makerWordEdit->clear();
    }
}

void MainWindow::showWord(Packet recPacket) {
    if(recPacket.signalType == 2) {
        ui->wordBrowser->setText(QString::fromStdString(recPacket.word));
    } else if(recPacket.signalType == -2) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("拉取单词失败"));
    }
}

void MainWindow::on_signupButton_clicked() {
    user.signoutUser();
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
        user.insertUser();
    } else if(username == "") {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不能为空"));
        user.signoutUser();
    } else if(password != repassword) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("两次密码输入不一致"));
        user.signoutUser();
    }
}


void MainWindow::on_signupRepassword_returnPressed() {
    user.signoutUser();
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
        user.insertUser();
    } else if(username == "") {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户名不能为空"));
        user.signoutUser();
    } else if(password != repassword) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("两次密码输入不一致"));
        user.signoutUser();
    }
}

void MainWindow::on_signinButton_clicked() {
    user.signoutUser();
    QString username = ui->signinUsername->text();
    QString password = ui->signinPassword->text();
    if(ui->signinBreakerButton->isChecked())
        user.userType = 1;
    else
        user.userType = 0;
    user.username = username;
    user.password = password;
    user.checkUser();
}

void MainWindow::on_signinPassword_returnPressed() {
    user.signoutUser();
    QString username = ui->signinUsername->text();
    QString password = ui->signinPassword->text();
    if(ui->signinBreakerButton->isChecked())
        user.userType = 1;
    else
        user.userType = 0;
    user.username = username;
    user.password = password;
    user.checkUser();
}

void MainWindow::on_gosigninButton_clicked() {
    ui->signupUsername->clear();
    ui->signupPassword->clear();
    ui->signupRepassword->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_gosignupButton_clicked() {
    ui->signinUsername->clear();
    ui->signinPassword->clear();
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

void MainWindow::on_startButton_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_submitButton_clicked() {
    if(!QString::compare(ui->makerWordEdit->text(), ""))
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("单词不能为空"));
    else
        game.updateWord(ui->makerWordEdit->text().toStdString());
}

void MainWindow::on_makerWordEdit_returnPressed() {
    if(!QString::compare(ui->makerWordEdit->text(), ""))
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("单词不能为空"));
    else
        game.updateWord(ui->makerWordEdit->text().toStdString());
}

void MainWindow::on_makerEndButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}
