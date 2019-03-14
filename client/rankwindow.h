#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <QDialog>

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
};

#endif // RANKWINDOW_H
