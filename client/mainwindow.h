#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_signupButton_clicked();
    void on_signinButton_clicked();
    void on_gosigninButton_clicked();
    void on_gosignupButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
