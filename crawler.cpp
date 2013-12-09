#include "crawler.h"
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QTimer>
#include "SystemInfo.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDir>
#include <assert.h>
Crawler::Crawler(QObject *parent) :
    QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbfile = QDir::home().absoluteFilePath(".watchme.sqlite");
    db.setDatabaseName(dbfile);
    assert(db.open());
    QSqlQuery("CREATE TABLE watch(timestamp INGEGER PRIMARY KEY, wclass TEXT, wtitle TEXT)").exec();

    QTimer * timer = new QTimer;
    timer->setInterval(6000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start();


}

void Crawler::onTimer()
{
    WindowInfo wi = getWindowInfo();
    QString wc = wi.windowClass.replace("'","''");
    QString wn = wi.windowName.replace("'","''");
    QSqlQuery(QString("INSERT INTO watch(timestamp,wclass,wtitle) VALUES(%1,'%2','%3')")
              .arg(QDateTime::currentMSecsSinceEpoch()/1000).arg(wc,wn))
            .exec();
}

QList<WindowInfo> Crawler::all(int seconds_before)
{
    qint64 ts = QDateTime::currentDateTime().addSecs(-seconds_before).toMSecsSinceEpoch()/1000;
    QSqlQuery q(QString("SELECT timestamp, wclass, wtitle FROM watch WHERE timestamp > %1").arg(ts));
    QList<WindowInfo> ans;
    if(!q.exec()) return ans;
    while(q.next())
    {
        WindowInfo i;
        i.windowClass = q.value(1).toString();
        i.windowName = q.value(2).toString();
        i.windowTitle = q.value(2).toString();
        ans << i;
    }
    return ans;
}
