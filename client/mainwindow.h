#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "user.h"
#include "rankwindow.h"
#include "finderwindow.h"

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
    void on_signupButton_clicked();
    void on_signinButton_clicked();
    void on_gosigninButton_clicked();
    void on_gosignupButton_clicked();
    void on_signoutButton_clicked();
    void on_findButton_clicked();
    void on_rankButton_clicked();
private:
    Ui::MainWindow *ui;
    RankWindow *rank;
    User user;
};

#endif // MAINWINDOW_H
