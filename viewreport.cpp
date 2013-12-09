#include "viewreport.h"
#include "ui_viewreport.h"
#include "nightcharts.h"
#include <QCryptographicHash>
ViewReport::ViewReport(QWidget *parent, Logic * lo, Crawler * cra) :
    QWidget(parent), logic(lo), crawler(cra),
    ui(new Ui::ViewReport)
{
    ui->setupUi(this);
}

ViewReport::~ViewReport()
{
    delete ui;
}

void ViewReport::update()
{
    show();
}

void ViewReport::paintEvent(QPaintEvent * e)
{
    logic->updateRules();
    QMap<QString, int> variants;
    int total = 0;
    foreach(WindowInfo wi, crawler->all(8*3600))
    {
        variants[logic->resolve(wi)]++;
        total++;
    }

    QWidget::paintEvent(e);
    QPainter painter;
    QFont font;
    painter.begin(this);
    Nightcharts PieChart;
    PieChart.setType(Nightcharts::Histogramm);//{Histogramm,Pie,DPie};
    PieChart.setLegendType(Nightcharts::Vertical);//{Round,Vertical}
    PieChart.setCords(this->width()/4,this->height()/8,this->width()/2,this->height()/2);

    /*PieChart.addPiece("Item1",QColor(200,10,50),34);
    PieChart.addPiece("Item2",Qt::green,27);
    PieChart.addPiece("Item3",Qt::cyan,14);
    PieChart.addPiece("Item4",Qt::yellow,7);
    PieChart.addPiece("Item5",Qt::blue,4);*/
    foreach(QString name, variants.keys())
    {
        QByteArray rcolor = QCryptographicHash::hash(name.toUtf8(), QCryptographicHash::Md5).toHex().left(6);
        QString scolor = QString("#%1").arg(QString::fromUtf8(rcolor));
        PieChart.addPiece(name, QColor(scolor),100*variants[name]/total);
    }

    PieChart.draw(&painter);
    PieChart.drawLegend(&painter);

}
