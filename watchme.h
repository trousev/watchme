#ifndef WATCHME_H
#define WATCHME_H

#include <QMainWindow>
#include <QDateTime>
#include <QTableWidgetItem>
#include <QDir>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QRegExp>
#include <QSystemTrayIcon>
#include "classificator.h"

namespace Ui {
class WatchMe;
}

class WatchMe : public QMainWindow
{
    Q_OBJECT

public:
    explicit WatchMe(QWidget *parent = 0);
    ~WatchMe();
    void populate(QDateTime start = QDateTime(), QDateTime end=QDateTime(), bool group=false);

private slots:
    void on__log_clicked(const QModelIndex &index);

    void on__log_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void on__reload_clicked();

    void on__add_clicked();

    void on__classification_currentTextChanged(const QString &arg1);

    void on__save_clicked();

    void on__remove_clicked();

    void on__log_doubleClicked(const QModelIndex &index);

    void on_actionQuit_triggered();

private:
    Ui::WatchMe *ui;
    CItemList currentClassification;
    QString currentClassificationName;
    QStringList getClassificationList();
    void reload(QString newName);
    void save(QString newName);
    void updateClassfication();
    void compile();
    QSystemTrayIcon * _tray;
};

#endif // WATCHME_H
