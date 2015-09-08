#include "crawler.h"
#include <QDir>
#include <QTimer>
#include <QDebug>
#include "SystemInfo.h"
Crawler::Crawler(QObject *parent) :
    QObject(parent)
{
    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start();
    _report.setFileName( QDir::home().absoluteFilePath(".watchme.log"));
    _report.open(QFile::Append);
}

Crawler::~Crawler()
{
    _report.close();
}

void Crawler::onTimer()
{
    WindowInfo i = getWindowInfo();
    if(_last_changed.isNull())
    {
        _last_changed = QDateTime::currentDateTime();
        _last_wi = i;
        return ;
    }
    if(i == _last_wi)
        return ;

    QDateTime cdt = QDateTime::currentDateTime();
    int delta = _last_changed.msecsTo(cdt);
    qDebug() << _last_wi.windowName << delta;
    if(_last_wi.isSystemIsIdle)
        _report.write(QString("%2 %1 IDLE\n").arg(delta).arg(cdt.toMSecsSinceEpoch()/1000).toUtf8());
    else if(_last_wi.windowClass.isEmpty())
        _report.write(QString("%2 %1 EMPTY\n").arg(delta).arg(cdt.toMSecsSinceEpoch()/1000).toUtf8());
    else
        _report.write(QString("%4 %1 class:%3 %2 \n").arg(delta).arg(_last_wi.windowName, _last_wi.windowClass).arg(cdt.toMSecsSinceEpoch()/1000).toUtf8());
    _report.flush();
    _last_changed = cdt;
    _last_wi = i;
}
