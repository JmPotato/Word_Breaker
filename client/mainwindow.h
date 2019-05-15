#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "user.h"
#include "game.h"
#include "rank.h"

#include <unistd.h>

#include <QTimer>
#include <QLineEdit>
#include <QVariant>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void signup(Packet recPacket);
    void signin(Packet recPacket);
    void getInfo(Packet recPacket);
    void showWord(Packet recPacket);
    void hideWord();
    void outWord();
    void updateWordList(Packet recPacket);
    void updateRank(Packet recPacket);
    void searchInfo(Packet recPacket);
    void on_signupButton_clicked();
    void on_signinButton_clicked();
    void on_gosigninButton_clicked();
    void on_gosignupButton_clicked();
    void on_signoutButton_clicked();
    void on_findButton_clicked();
    void on_rankButton_clicked();
    void on_startButton_clicked();
    void on_makerWordEdit_returnPressed();
    void on_signinPassword_returnPressed();
    void on_signupRepassword_returnPressed();
    void on_makerEndButton_clicked();
    void on_breakerWordEdit_returnPressed();
    void on_breakerEndButton_clicked();

    void on_backButton_clicked();

    void on_searchButton_clicked();

    void on_searchBackButton_clicked();

private:
    Ui::MainWindow *ui;

    User user;
    Game game;
    Rank rank;

    unsigned short countDown = 5;
    QTimer *hideTimer = new QTimer(this);
    QTimer *outTimer = new QTimer(this);

};

#endif // MAINWINDOW_H
