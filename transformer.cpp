#include "transformer.h"
#include "ui_transformer.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

Transformer::Transformer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transformer)
{
    ui->setupUi(this);
}

Transformer::~Transformer()
{
    delete ui;
}


void Transformer::loadEntries()
{

}

void Transformer::loadRules()
{

}
