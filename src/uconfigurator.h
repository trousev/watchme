#ifndef UCONFIGURATOR_H
#define UCONFIGURATOR_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
namespace Ui {
class uConfigurator;
}

class uConfigurator : public QMainWindow
{
    Q_OBJECT

    QSqlQueryModel * filtersModel;
    QSqlQueryModel * sheetModel;
    QSortFilterProxyModel * sheetFilterTitleModel;
    QSortFilterProxyModel * sheetFilterClassModel;
    QSortFilterProxyModel * sheetFilterSort;
public:
    explicit uConfigurator(QWidget *parent = 0);
    ~uConfigurator();

private slots:

    void on__wtitle_textEdited(const QString &arg1);

    void on__wclass_textEdited(const QString &arg1);

    void on__add_clicked();
    void refresh();
    void rehide();

    void on__remove_clicked();

    void on__up_clicked();
    void swap(QModelIndex i, QModelIndex j);

    void on__down_clicked();

private:
    Ui::uConfigurator *ui;
};

#endif // UCONFIGURATOR_H