#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include "client.h"

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
    class RankWindow;
}

class RankWindow : public QDialog {
    Q_OBJECT
public:
    explicit RankWindow(QWidget *parent = nullptr);
    ~RankWindow();
public slots:
    void setRank(QByteArray data);
private:
    Ui::RankWindow *ui;
};

#endif // RANKWINDOW_H
