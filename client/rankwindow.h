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
private:
    Ui::RankWindow *ui;
public slots:
    void setRank(QByteArray data);
};

#endif // RANKWINDOW_H
