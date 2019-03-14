#ifndef FINDERWINDOW_H
#define FINDERWINDOW_H

#include <QDialog>

namespace Ui {
    class FinderWindow;
}

class FinderWindow : public QDialog {
    Q_OBJECT
public:
    explicit FinderWindow(QWidget *parent = nullptr);
    ~FinderWindow();
private:
    Ui::FinderWindow *ui;
};

#endif // FINDERWINDOW_H
