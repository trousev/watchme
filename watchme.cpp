#include "watchme.h"
#include "ui_watchme.h"
#include "crawler.h"
#include <QTextStream>
#include <QHash>
#include <QDebug>
#include <QFile>
#include <QDir>

WatchMe::WatchMe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WatchMe)
{
    ui->setupUi(this);
    ui->_toolbar->addWidget(ui->_from);
    ui->_toolbar->addWidget(ui->_to);
    ui->_toolbar->addWidget(ui->_syndicate);
    ui->_toolbar->addSeparator();
    ui->_toolbar->addWidget(ui->_classification);
    ui->_toolbar->addWidget(ui->_add);
    ui->_toolbar->addWidget(ui->_remove);
    ui->_toolbar->addWidget(ui->_reload);

    ui->_classification->clear();
    ui->_classification->addItems(getClassificationList());
    updateClassfication();
    populate(ui->_from->dateTime(), ui->_to->dateTime(), ui->_syndicate->isChecked());
    compile();
}

WatchMe::~WatchMe()
{
    save(ui->_classification->currentText());
    delete ui;
}

void WatchMe::populate(QDateTime start, QDateTime end, bool group)
{
    qDebug() << ui->_filters->rowCount();
    ui->_log->clear();
    ui->_log->setSortingEnabled(false);
    ui->_log->setRowCount(0);
    QFile _report;
    _report.setFileName( Crawler::mainDir().absoluteFilePath(".watchme.log"));
    _report.open(QFile::ReadOnly);
    QTextStream stream(&_report);
    QHash<QString, int> _index;
    while(true)
    {
        qint64 timestamp, duration;
        stream >> timestamp >> duration;
        QString line = stream.readLine();
        if(line.isEmpty() && _report.atEnd()) break;
        line = line.replace("\n","");
        line = line.replace("\r","");

        if (_index.contains(line) && group)
        {
            int rowNo = _index[line];
            QTableWidgetItem * date_item = ui->_log->item(rowNo,0);
            duration += date_item->data(Qt::UserRole+1).toInt();
            date_item->setData(Qt::UserRole+1, duration);
            int secs = duration/1000;
            ui->_log->item(rowNo,1)->setText(QString("%1:%2").arg(secs/60 ,2,10,QChar('0')).arg(secs%60,2,10,QChar('0')));
        }
        else
        {
            ui->_log->insertRow(ui->_log->rowCount());

            QTableWidgetItem * date_item = new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(timestamp*1000).toString("d.MM, hh:mm"));
            date_item->setData(Qt::UserRole, timestamp);
            date_item->setData(Qt::UserRole+1, duration);
            date_item->setData(Qt::UserRole+2, line);
            int secs = duration/1000;
            ui->_log->setItem(ui->_log->rowCount()-1,0,date_item );
            ui->_log->setItem(ui->_log->rowCount()-1,1,new QTableWidgetItem(QString("%1:%2").arg(secs/60 ,2,10,QChar('0')).arg(secs%60,2,10,QChar('0'))  ) );
            ui->_log->setItem(ui->_log->rowCount()-1,3,new QTableWidgetItem(line));
            _index[line] = ui->_log->rowCount()-1;
        }
    }
    qDebug() << ui->_log->rowCount();
    ui->_log->setSortingEnabled(true);
    compile();
}

void WatchMe::on__log_clicked(const QModelIndex &index)
{
    ui->_log_line->setText(ui->_log->item(ui->_log->currentRow(),3)->text());
}

void WatchMe::on__log_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if(current)
        ui->_log_line->setText(ui->_log->item(ui->_log->currentRow(),3)->text());
}

void WatchMe::on__reload_clicked()
{
    currentClassification.clear();
    for(int i=0; i<ui->_filters->rowCount(); i++)
    {
        currentClassification << CItem(ui->_filters->item(i,0)->text(),ui->_filters->item(i,1)->text());
    }
    save(ui->_classification->currentText());
    compile();
    populate(ui->_from->dateTime(), ui->_to->dateTime(), ui->_syndicate->isChecked());
}

void WatchMe::on__add_clicked()
{
    ui->_filters->insertRow(ui->_filters->rowCount());
    ui->_filters->setItem(ui->_filters->rowCount()-1, 0, new QTableWidgetItem("Category/Subcategory"));
    ui->_filters->setItem(ui->_filters->rowCount()-1, 1, new QTableWidgetItem("Matching regex here..."));
}

QStringList WatchMe::getClassificationList() {
    QStringList ans;
    foreach(QFileInfo fi, Crawler::mainDir().entryInfoList(QStringList() << "*.classification.xml")) {
        ans << fi.baseName();
    }
    if(!ans.contains("default"))
        ans << "default";
    return ans;
}
void WatchMe::reload(QString newName)
{
    QFile f(Crawler::mainDir().absoluteFilePath(QString("%1.classification.xml").arg(newName)));

    currentClassification.clear();
    // TODO: ui cleaning
    ui->_filters->clear();

    QDomDocument doc;
    if(!f.open(QFile::ReadOnly))
        return ;
    if(!doc.setContent(&f))
    {
        f.close();
        return ;
    }
    f.close();

    currentClassification = CItemList(doc.documentElement());
    foreach(CItem i, currentClassification)
    {
        ui->_filters->insertRow(ui->_filters->rowCount());
        ui->_filters->setItem(ui->_filters->rowCount()-1, 0, new QTableWidgetItem(i.name()));
        ui->_filters->setItem(ui->_filters->rowCount()-1, 1, new QTableWidgetItem(i.query().pattern()));
    }

}

void WatchMe::save(QString newName)
{
    if(newName.isEmpty()) return ;
    QFile f(Crawler::mainDir().absoluteFilePath(QString("%1.classification.xml").arg(newName)));
    f.open(QFile::WriteOnly);
    QDomDocument doc("watchme");
    doc.appendChild(currentClassification.toDomElement(doc));
    f.write(doc.toString(2).toUtf8());
    f.close();
}

void WatchMe::updateClassfication() {
    QString newName = ui->_classification->currentText();
    save(currentClassificationName);

    if(newName == currentClassificationName)
        return ;
    currentClassificationName = newName;
    reload(currentClassificationName);
}

void WatchMe::on__classification_currentTextChanged(const QString &arg1)
{
    updateClassfication();
}
QColor __getColor(int number) {
    int hue_main = number % 3;
    int hue_offset = (number/3) % 2;
    int hue_offset2 = (number/6) % 2;
    int hue = hue_main * 120 + hue_offset * 60 + hue_offset2 * 30;
    hue = hue % 360;
    int saturation_flag = (number/12)%2;
    int sat = 255 - saturation_flag * (128+64);
    return QColor::fromHsl(hue,sat,128);
}

void WatchMe::compile() {
    ui->_chart->clear();
    ui->_chart->setType(Nightcharts::Pie);
    CReport report;
    for(int row=0; row<ui->_log->rowCount(); row++)
    {
        QString probe = ui->_log->item(row,0)->data(Qt::UserRole+2).toString();
        int time = ui->_log->item(row,0)->data(Qt::UserRole+1).toInt();
        QString match = currentClassification.getMatch(probe, "Other/Unknown");
        report.track(match, time);
        ui->_log->setItem(row,2, new QTableWidgetItem( match ));
    }
    QMap<QString, float> _result = report.result();
    for(int i=0; i<_result.keys().count(); i++)
    {
        QString key = _result.keys().at(i);
        float value = _result.values().at(i);
        ui->_chart->addItem(key, __getColor(i), value);
    }
    ui->_chart->repaint();
}

void WatchMe::on__save_clicked()
{

}

void WatchMe::on__remove_clicked()
{
    if(ui->_filters->currentRow() < 0) return ;
    if(ui->_filters->currentRow() >= ui->_filters->rowCount()) return ;
    ui->_filters->removeRow(ui->_filters->currentRow());
}
