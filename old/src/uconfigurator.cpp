#include "uconfigurator.h"
#include "ui_uconfigurator.h"
#include <QDateTime>
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
    sheetModel = new QSqlQueryModel;

    multiplier = 1;

    sheetFilterTitleModel = new QSortFilterProxyModel;
    sheetFilterTitleModel->setFilterKeyColumn(1);
    sheetFilterClassModel = new QSortFilterProxyModel;
    sheetFilterClassModel->setFilterKeyColumn(2);
    sheetFilterSort = new QSortFilterProxyModel;

    sheetFilterClassModel->setSourceModel(sheetModel);
    sheetFilterTitleModel->setSourceModel(sheetFilterClassModel);
    sheetFilterSort->setSourceModel(sheetFilterTitleModel);

    ui->_sheet->setModel(sheetFilterSort);
    connect(ui->_slider, SIGNAL(valueChanged(int)), this, SLOT(onSlider()));
}
void uConfigurator::appear()
{
    show();
    onSlider();
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
    q.exec(QString("INSERT INTO rules(wclass, wtitle, pattern, level) VALUES('%1', '%2', '%3', %4);")
           .arg(ui->_wclass->text())
           .arg(ui->_wtitle->text())
           .arg(ui->_pattern->currentText())
           .arg(no)
    );
    refresh();
}
void uConfigurator::setLimits(int mn, int mx)
{
    qDebug() <<"Limits" << minimum << maximum;
    minimum = mn;
    maximum = mx;
    refresh();
}

void uConfigurator::refresh()
{

    filtersModel->setQuery("SELECT rowid,wtitle,wclass,level,pattern,wtitle||' ('||wclass||')' FROM rules ORDER BY level ASC; ");
    ui->_filters->setModel(filtersModel);

    QFile fsheetq(":sql/sheet.sql");
    fsheetq.open(QFile::ReadOnly);
    QString sheetq = QString::fromUtf8(fsheetq.readAll());
    fsheetq.close();
    sheetq = sheetq.replace("{minimum}",QString("%1").arg(minimum));
    sheetq = sheetq.replace("{maximum}",QString("%1").arg(maximum));
    //qDebug() << sheetq;
    sheetModel->setQuery(sheetq);

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
    filtersModel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Pattern"));
    filtersModel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("Query"));
    ui->_filters->hideColumn(0);
    ui->_filters->hideColumn(1);
    ui->_filters->hideColumn(2);
    ui->_filters->hideColumn(3);
    setHtml("pie");
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
    //qDebug() << q.lastQuery();
    //qDebug() << q.lastError().text();

    q.exec(QString("UPDATE rules SET level = '%1' WHERE rowid= '%2'; ")
           .arg(l_j.data().toString())
           .arg(r_i.data().toString())
           );
    //qDebug() << q.lastQuery();
    //qDebug() << q.lastError().text();
    q.exec("COMMIT;");
    q.exec("ROLLBACK;");
}

void uConfigurator::on__up_clicked()
{
    QModelIndex me = ui->_filters->currentIndex();
    QModelIndex he = me.sibling(me.row()-1, me.column());
    //qDebug() << me;
    //qDebug() << he;
    swap(me, he);
    refresh();
}

void uConfigurator::on__down_clicked()
{
    QModelIndex me = ui->_filters->currentIndex();
    QModelIndex he = me.sibling(me.row()+1, me.column());
    //qDebug() << me;
    //qDebug() << he;
    swap(me, he);
    refresh();
}

void uConfigurator::setHtml(QString name)
{
    QString ans = "Unknown";
    QString data;

    QFile fsheetq(":sql/pattern.sql");
    fsheetq.open(QFile::ReadOnly);
    QString sheetq = QString::fromUtf8(fsheetq.readAll());
    fsheetq.close();
    sheetq = sheetq.replace("{minimum}",QString("%1").arg(minimum));
    sheetq = sheetq.replace("{maximum}",QString("%1").arg(maximum));

    QSqlQuery q(sheetq);
    QStringList datas;
    QColor c;
    int size = 0;
    while(q.next()) size ++;
    q.seek(-1);
    if(size <= 0) size = 1;
    int step = 300/size;
    int hue = 0;
    int saturation = 100;
    int bright = 100;
    while(q.next())
    {
        c.setHsl(hue,saturation, bright);
        int value = q.value(0).toInt();
        QString pattern = q.value(1).toString();
        if(pattern.isEmpty()) pattern = "Unknown actions";
        datas << QString("{value : %1,color: \"%3\",title : \"%2\"}").arg(value).arg(pattern).arg(c.name());
        //qDebug() << c.name() << hue << saturation << bright;
        hue += step;
    }
    data = QString("[%1]").arg(datas.join(","));

    if(name == "pie")
    {
        QFile t(":chartjs/index.html");
        t.open(QFile::ReadOnly);
        ans = QString::fromUtf8(t.readAll());
        t.close();
        ans = ans.replace("{pie_data}", data);
        //ans = data;
    }
    ui->_report->setHtml(ans);
}

void uConfigurator::on__today_clicked()
{
    setLimits(
                QDateTime(QDate::currentDate()).secsTo(QDateTime::currentDateTime())
                , 0
                );
}

void uConfigurator::on__yesterday_clicked()
{
    QDate yesterday = QDate::currentDate();
    yesterday =  yesterday.addDays(-7);
    setLimits(
                QDateTime(yesterday).secsTo(QDateTime::currentDateTime())
                ,
                QDateTime(QDate::currentDate()).secsTo(QDateTime::currentDateTime())
                );
}

void uConfigurator::onSlider()
{
    int day = ui->_slider->value();
    qDebug() << day;
    QDate today = QDate::currentDate();
    QDate max = today.addDays(- day * multiplier + 1);
    QDate min = max.addDays(-multiplier);
    if(multiplier == 1)
        ui->_slider_label->setText(QString("%1").arg(min.toString()));
    else
        ui->_slider_label->setText(QString("%1 - %2").arg(min.toString()).arg(max.addDays(-1).toString()));
    QDateTime now = QDateTime::currentDateTime();
    setLimits( QDateTime(min).secsTo(now), QDateTime(max).secsTo(now) );
}

void uConfigurator::on__m_selector_clicked()
{
    if(multiplier == 1)
    {
        ui->_m_selector->setText("weeks");
        multiplier = 7;
    }

    else
    {
        ui->_m_selector->setText("days");
        multiplier = 1;
    }
    onSlider();
}
