#ifndef CRAWLER_H
#define CRAWLER_H

#include <QDir>
#include <QObject>
#include <QFile>
#include "SystemInfo.h"
#include <QDateTime>
class Crawler : public QObject
{
    Q_OBJECT
public:
    explicit Crawler(QObject *parent = 0);
    ~Crawler();
signals:

public slots:
    void onTimer();
private:

    QFile _report;
    WindowInfo _last_wi;
    QDateTime _last_changed;
public:
    static QDir mainDir();
};

#endif // CRAWLER_H
