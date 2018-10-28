#ifndef UWATCHME_H
#define UWATCHME_H

#include <QDialog>
#include "utracker.h"

namespace Ui {
class UWatchMe;
}

class UWatchMe : public QDialog
{
    Q_OBJECT
    UTracker * _tracker;
public:
    explicit UWatchMe(UTracker * tracker, QWidget *parent = 0);
    ~UWatchMe();

private slots:
    void on__login_clicked();

    void on__logout_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UWatchMe *ui;
    void reloadUi();
signals:
    void somethingChanged(QString title, QString message);
};

#endif // UWATCHME_H
