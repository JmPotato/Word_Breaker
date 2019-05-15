#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->signinBreakerButton->toggle();
    ui->signupBreakerButton->toggle();
    ui->signinPassword->setEchoMode(QLineEdit::Password);
    ui->signupPassword->setEchoMode(QLineEdit::Password);
    ui->signupRepassword->setEchoMode(QLineEdit::Password);
    ui->message->setAlignment(Qt::AlignCenter);

    ui->timerBar->setRange(0, game.currentTimeLimted);
    ui->timerBar->setValue(game.currentTimeLimted);

    ui->rankTable->setRowCount(10);
    ui->rankTable->setColumnCount(4);
    ui->rankTable->setHorizontalHeaderLabels({"用户名","单词数","经验值","闯关数"});

    ui->signupUsername->setFocus();
    ui->signinUsername->setFocus();

    ui->stackedWidget->setCurrentIndex(1);

    connect(hideTimer, SIGNAL(timeout()), this, SLOT(hideWord()));
    connect(outTimer, SIGNAL(timeout()), this, SLOT(outWord()));
    connect(&user, &User::signupSignal, this, &MainWindow::signup);
    connect(&user, &User::signinSignal, this, &MainWindow::signin);
    connect(&user, &User::getInfoSignal, this, &MainWindow::getInfo);
    connect(&user, &User::getResultSignal, this, &MainWindow::searchInfo);
    connect(&game, &Game::getWordSignal, this, &MainWindow::showWord);
    connect(&game, &Game::updateWordSignal, this, &MainWindow::updateWordList);
    connect(&rank, &Rank::reciveUserSignal, this, &MainWindow::updateRank);
}

MainWindow::~MainWindow() {
    user.signoutUser();
    delete ui;
}

/*登陆功能响应模块*/
void MainWindow::signup(Packet recPacket) {
    if(recPacket.signalType == 1) {
        ui->signupUsername->clear();
        ui->signupPassword->clear();
        ui->signupRepassword->clear();
        user.signinUser();
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
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登陆失败：用户名与密码不匹配或该账号重复登陆"));
    }
}

void MainWindow::getInfo(Packet recPacket) {
    if(recPacket.signalType == 3) {
        user.mark = recPacket.mark;
        user.xp = recPacket.xp;
        user.level = recPacket.level;
        ui->message->setText(QString("用户名：%1    单词数：%2    经验值：%3    已闯关数：%4").arg(user.username).arg(user.mark).arg(user.xp).arg(user.level));
    } else if(recPacket.signalType == -3) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("获取用户信息失败"));
        user.signoutUser();
    }
}

/*游戏功能响应模块*/
void MainWindow::updateWordList(Packet recPacket) {
    if(recPacket.signalType == 1) {
        if(!game.wordList.empty() && game.wordList.count() % 3 == 0)
            game.currentLevel++;
        ui->wordList->addItem(QString::fromStdString(recPacket.word));
        ui->wordList->scrollToBottom();
        user.mark++;
        user.xp += QString::fromStdString(recPacket.word).length();
        if(game.currentLevel > user.level)
            user.level = game.currentLevel;
    } else if(recPacket.signalType == -1) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("单词已存在"));
    }
    ui->makerWordEdit->clear();
}

void MainWindow::showWord(Packet recPacket) {
    ui->breakerWordEdit->setEnabled(false);
    ui->timerBar->setRange(0, game.currentTimeLimted);
    ui->timerBar->setValue(game.currentTimeLimted);
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
            ui->levelLabel->setText(QString("关卡：%1").arg(game.currentLevel));
            ui->wordLabel->setText(QString::fromStdString(recPacket.word));
            hideTimer->start(1500);
        }
    } else if(recPacket.signalType == -2) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("恭喜您完成本次闯关"));
    }
}

void MainWindow::hideWord() {
    ui->wordLabel->clear();
    ui->breakerWordEdit->setEnabled(true);
    ui->breakerWordEdit->setFocus();
    outTimer->start(1000);
}

void MainWindow::outWord() {
    ui->timerBar->setValue(--countDown);
    if(countDown == 0) {
        user.updateUser();
        user.signinUser();
        outTimer->stop();
        game.endGame();
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("超时淘汰"));
        ui->stackedWidget->setCurrentIndex(2);
    }
}

/*排行榜功能响应模块*/
void MainWindow::updateRank(Packet recPacket) {
    static int number = 0;
    if(recPacket.signalType == 1) {
        QTableWidgetItem *itemUsername = new QTableWidgetItem;
        itemUsername->setData(Qt::DisplayRole, QString(recPacket.username));
        ui->rankTable->setItem(number, 0, itemUsername);
        QTableWidgetItem *itemMark = new QTableWidgetItem;
        itemMark->setData(Qt::DisplayRole, recPacket.mark);
        ui->rankTable->setItem(number, 1, itemMark);
        QTableWidgetItem *itemXp = new QTableWidgetItem;
        itemXp->setData(Qt::DisplayRole, recPacket.xp);
        ui->rankTable->setItem(number, 2, itemXp);
        QTableWidgetItem *itemLevel = new QTableWidgetItem;
        itemLevel->setData(Qt::DisplayRole, recPacket.level);
        ui->rankTable->setItem(number, 3, itemLevel);
        number++;
    } else if(recPacket.signalType == -1) {
        number = 0;
        ui->rankTable->sortItems(2, Qt::DescendingOrder);
        ui->rankTable->sortItems(1, Qt::DescendingOrder);
        ui->rankTable->sortItems(3, Qt::DescendingOrder);
        ui->stackedWidget->setCurrentIndex(5);
//        ui->rankTable->show();
    }
}

/*搜索功能响应模块*/
void MainWindow::searchInfo(Packet recPacket) {
    if(recPacket.signalType == 6) {
        ui->searchEdit->clear();
        ui->xpResult->setText(QString::number(recPacket.xp));
        ui->markResult->setText(QString::number(recPacket.mark));
        ui->levelResult->setText(QString::number(recPacket.level));
        ui->usernameResult->setText(QString(recPacket.username));
    } else if(recPacket.signalType == -6) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("查无此人"));
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

/*主界面*/
void MainWindow::on_signoutButton_clicked() {
    user.signoutUser();
    ui->rankTable->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_rankButton_clicked() {
    rank.getRank(user.userType);
//    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_findButton_clicked() {
    ui->makerRadio->toggle();
    ui->stackedWidget->setCurrentIndex(6);
}

/*游戏界面*/
void MainWindow::on_startButton_clicked() {
    if(user.userType == 1) {
        game.getWord(game.difficulty);
        countDown = game.currentTimeLimted;
        ui->stackedWidget->setCurrentIndex(4);
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
    user.updateUser();
    user.signinUser();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_breakerWordEdit_returnPressed() {
    if(ui->breakerWordEdit->text() == game.wordList.last()) {
        if(!game.wordList.empty() && game.wordList.count() % 3 == 0)
            game.currentLevel++;
        if(!game.wordList.empty() && game.wordList.count() > 10)
            game.currentTimeLimted = 4;
        if(!game.wordList.empty() && game.wordList.count() > 20)
            game.currentTimeLimted = 3;
        hideTimer->stop();
        outTimer->stop();
        countDown = game.currentTimeLimted;
        user.mark++;
        user.xp += game.wordList.last().length();
        if(game.wordList.count() / 5 != 0 && game.wordList.count() % 5 == 0)
            game.difficulty++;
        game.getWord(game.difficulty);
        if(game.currentLevel > user.level)
            user.level = game.currentLevel;
    }
    ui->breakerWordEdit->clear();
}

void MainWindow::on_breakerEndButton_clicked() {
    ui->breakerWordEdit->clear();
    user.updateUser();
    user.signinUser();
    hideTimer->stop();
    outTimer->stop();
    game.endGame();
    ui->stackedWidget->setCurrentIndex(2);
}

/*排行榜界面*/
void MainWindow::on_backButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2);

}

/* 玩家查询界面 */
void MainWindow::on_searchButton_clicked() {
    if(!QString::compare(ui->searchEdit->text(), "")) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("查找用户名不能为空"));
    } else {
        if(ui->makerRadio->isChecked())
            user.searchUser(ui->searchEdit->text(), 0);
        else
            user.searchUser(ui->searchEdit->text(), 1);
    }
}

void MainWindow::on_searchBackButton_clicked() {
    ui->searchEdit->clear();
    ui->xpResultLabel->clear();
    ui->markResultLabel->clear();
    ui->levelResultLabel->clear();
    ui->usernameResultLabel->clear();
    ui->stackedWidget->setCurrentIndex(2);
}
