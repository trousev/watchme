#include "uaddfilter.h"
#include "ui_uaddfilter.h"

uAddFilter::uAddFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uAddFilter)
{
    ui->setupUi(this);
}

uAddFilter::~uAddFilter()
{
    delete ui;
}
