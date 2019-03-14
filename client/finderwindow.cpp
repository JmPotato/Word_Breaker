#include "finderwindow.h"
#include "ui_finderwindow.h"

FinderWindow::FinderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinderWindow)
{
    ui->setupUi(this);
}

FinderWindow::~FinderWindow()
{
    delete ui;
}
