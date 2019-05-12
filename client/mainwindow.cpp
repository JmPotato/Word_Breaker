#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->signinBreakerButton->toggle();
    ui->signupBreakerButton->toggle();
    ui->stackedWidget->setCurrentIndex(1);
    ui->message->setAlignment(Qt::AlignCenter);
    ui->signinPassword->setEchoMode(QLineEdit::Password);
    ui->signupPassword->setEchoMode(QLineEdit::Password);
    ui->signupRepassword->setEchoMode(QLineEdit::Password);
    ui->signinUsername->setFocus();
    ui->signupUsername->setFocus();
    ui->timerBar->setRange(0, 5);
    ui->timerBar->setValue(5);

    connect(hideTimer, SIGNAL(timeout()), this, SLOT(hideWord()));
    connect(outTimer, SIGNAL(timeout()), this, SLOT(outWord()));
    connect(&user, &User::signupSignal, this, &MainWindow::signup);
    connect(&user, &User::signinSignal, this, &MainWindow::signin);
    connect(&user, &User::getInfoSignal, this, &MainWindow::getInfo);
    connect(&game, &Game::getWordSignal, this, &MainWindow::showWord);
    connect(&game, &Game::updateWordSignal, this, &MainWindow::updateWordList);
}

MainWindow::~MainWindow() {
    delete ui;
}

/*登陆功能响应模块*/
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
        user.mark = recPacket.mark;
        user.xp = recPacket.xp;
        user.level = recPacket.level;
        ui->message->setText(QString("Username: %1 Mark: %2 Xp: %3 Level: %4").arg(user.username).arg(user.mark).arg(user.xp).arg(user.level));
    } else if(recPacket.signalType == -3) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("获取用户信息失败"));
        user.signoutUser();
    }
}

/*游戏功能响应模块*/
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
    static unsigned short retryTime = 0;
    if(recPacket.signalType == 2) {
        if(game.wordList.contains(QString::fromStdString(recPacket.word))) {
            if(retryTime <= 5)
                retryTime++;
            else {
                game.difficulty++;
                retryTime = 0;
            }
            game.getWord(game.difficulty);
        } else {
            game.wordList.append(QString::fromStdString(recPacket.word));
            ui->wordLabel->setText(QString::fromStdString(recPacket.word));
            ui->breakerWordEdit->setEnabled(false);
            ui->timerBar->setValue(countDown);
            hideTimer->start(1500);
        }
    } else if(recPacket.signalType == -2) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("闯关成功"));
    }
}

void MainWindow::hideWord() {
    ui->wordLabel->clear();
    ui->breakerWordEdit->setEnabled(true);
    outTimer->start(1000);
}

void MainWindow::outWord() {
    ui->timerBar->setValue(--countDown);
    if(countDown == 0) {
        outTimer->stop();
        countDown = 5;
        game.getWord(game.difficulty);
    }
}

/*界面逻辑模块*/
/*登陆注册界面*/
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

/*主界面*/
void MainWindow::on_findButton_clicked() {
}

void MainWindow::on_rankButton_clicked() {
    rank = new RankWindow(this);
    rank->show();
}

/*游戏界面*/
void MainWindow::on_startButton_clicked() {
    if(user.userType == 1) {
        ui->stackedWidget->setCurrentIndex(4);
        game.difficulty = 2;
        game.wordList.clear();
        game.getWord(game.difficulty);
    } else if(user.userType == 0)
        ui->stackedWidget->setCurrentIndex(3);

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

void MainWindow::on_breakerWordEdit_returnPressed() {
    if(ui->breakerWordEdit->text() == game.wordList.last()) {
        outTimer->stop();
        countDown = 5;
        game.getWord(game.difficulty);
    }
    ui->breakerWordEdit->clear();
}

void MainWindow::on_breakerEndButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
    outTimer->stop();
    countDown = 5;
}
