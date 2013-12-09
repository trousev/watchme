#ifndef VIEWREPORT_H
#define VIEWREPORT_H

#include <QWidget>
#include "logic.h"
#include "crawler.h"

namespace Ui {
class ViewReport;
}

class ViewReport : public QWidget
{
    Q_OBJECT

public:
    explicit ViewReport(QWidget *parent, Logic * lo, Crawler * cra) ;
    ~ViewReport();
public slots:
    void update();
private:
    Ui::ViewReport *ui;
protected:
    void paintEvent(QPaintEvent * event);
    Logic * logic;
    Crawler * crawler;
};

#endif // VIEWREPORT_H
