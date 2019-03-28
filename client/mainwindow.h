#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "user.h"
#include "game.h"
#include "rankwindow.h"

#include <QLineEdit>
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
    void updateWordList(Packet recPacket);
    void on_signupButton_clicked();
    void on_signinButton_clicked();
    void on_gosigninButton_clicked();
    void on_gosignupButton_clicked();
    void on_signoutButton_clicked();
    void on_findButton_clicked();
    void on_rankButton_clicked();
    void on_startButton_clicked();
    void on_submitButton_clicked();

    void on_makerWordEdit_returnPressed();

    void on_signinPassword_returnPressed();

    void on_signupRepassword_returnPressed();

    void on_makerEndButton_clicked();

private:
    Ui::MainWindow *ui;
    RankWindow *rank;
    User user;
    Game game;
};

#endif // MAINWINDOW_H
