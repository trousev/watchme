#include "uconfigurator.h"
#include "ui_uconfigurator.h"
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>
uConfigurator::uConfigurator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::uConfigurator)
{
    ui->setupUi(this);
    filtersModel = new  QSqlQueryModel;
    filtersModel->setQuery("SELECT rowid,* FROM rules ORDER BY level ASC; ");
    ui->_filters->setModel(filtersModel);


    QFile fsheetq(":sql/sheet.sql");
    fsheetq.open(QFile::ReadOnly);
    QString sheetq = QString::fromUtf8(fsheetq.readAll());
    fsheetq.close();
    sheetModel = new QSqlQueryModel;
    sheetModel->setQuery(sheetq);

    sheetFilterTitleModel = new QSortFilterProxyModel;
    sheetFilterTitleModel->setFilterKeyColumn(1);
    sheetFilterClassModel = new QSortFilterProxyModel;
    sheetFilterClassModel->setFilterKeyColumn(2);
    sheetFilterSort = new QSortFilterProxyModel;

    sheetFilterClassModel->setSourceModel(sheetModel);
    sheetFilterTitleModel->setSourceModel(sheetFilterClassModel);
    sheetFilterSort->setSourceModel(sheetFilterTitleModel);

    //sheetFilterModel->setSourceModel(sheetModel);

    ui->_sheet->setModel(sheetFilterSort);
    rehide();
}

uConfigurator::~uConfigurator()
{
    delete ui;
}


void uConfigurator::on__wtitle_textEdited(const QString &text)
{
    //if(text.length() < 4) sheetFilterTitleModel->setFilterWildcard("");
    sheetFilterTitleModel->setFilterWildcard(text);
}

void uConfigurator::on__wclass_textEdited(const QString &text)
{
    //if(text.length() < 4) sheetFilterClassModel->setFilterWildcard("");
    sheetFilterClassModel->setFilterWildcard(text);
}

void uConfigurator::on__add_clicked()
{

    QSqlQuery q;
    q.exec("select cast(max(level) as integer)+1 from rules;");
    q.next();
    int no = q.value(0).toInt();
    q.exec(QString("INSERT INTO rules(wclass, wtitle, pattern, level) VALUES('%%1%', '%%2%', '%3', %4);")
           .arg(ui->_wclass->text())
           .arg(ui->_wtitle->text())
           .arg(ui->_pattern->currentText())
           .arg(no)
    );
    refresh();
}

void uConfigurator::refresh()
{
    QString fQ = filtersModel->query().lastQuery();
    QString sQ = sheetModel->query().lastQuery();
    filtersModel->setQuery(QString());
    sheetModel->setQuery(QString());
    filtersModel->setQuery(fQ);
    sheetModel->setQuery(sQ);
    rehide();
}
void uConfigurator::rehide()
{
    ui->_filters->hideColumn(0);
    //ui->_filters->hideColumn(3);
}

void uConfigurator::on__remove_clicked()
{
    QModelIndex i = ui->_filters->currentIndex();
    if(!i.isValid()) return ;
    i = i.sibling(i.row(), 0);
    if(!i.isValid()) return ;
    QSqlQuery q;
    q.exec(QString("DELETE FROM rules WHERE rowid=%1;").arg(i.data().toString()));
    refresh();
}
void uConfigurator::swap(QModelIndex i, QModelIndex j)
{
    QModelIndex l_i = i.sibling(i.row(),3);
    QModelIndex l_j = j.sibling(j.row(),3);
    QModelIndex r_i = i.sibling(i.row(),0);
    QModelIndex r_j = j.sibling(j.row(),0);
    if(!l_i.isValid()) return ;
    if(!l_j.isValid()) return ;
    if(!r_i.isValid()) return ;
    if(!r_j.isValid()) return ;

    QSqlQuery q;
    q.exec("BEGIN;");
    q.exec(QString("UPDATE rules SET level = '%1' WHERE rowid= '%2'; ")
           .arg(l_i.data().toString())
           .arg(r_j.data().toString())
           );
    qDebug() << q.lastQuery();
    qDebug() << q.lastError().text();

    q.exec(QString("UPDATE rules SET level = '%1' WHERE rowid= '%2'; ")
           .arg(l_j.data().toString())
           .arg(r_i.data().toString())
           );
    qDebug() << q.lastQuery();
    qDebug() << q.lastError().text();
    q.exec("COMMIT;");
    q.exec("ROLLBACK;");
}

void uConfigurator::on__up_clicked()
{
    QModelIndex me = ui->_filters->currentIndex();
    QModelIndex he = me.sibling(me.row()-1, me.column());
    qDebug() << me;
    qDebug() << he;
    swap(me, he);
    refresh();
}

void uConfigurator::on__down_clicked()
{
    QModelIndex me = ui->_filters->currentIndex();
    QModelIndex he = me.sibling(me.row()+1, me.column());
    qDebug() << me;
    qDebug() << he;
    swap(me, he);
    refresh();
}
