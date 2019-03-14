#include "rankwindow.h"
#include "ui_rankwindow.h"

RankWindow::RankWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RankWindow)
{
    ui->setupUi(this);
}

RankWindow::~RankWindow()
{
    delete ui;
}
