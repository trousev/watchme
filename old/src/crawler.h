#ifndef CRAWLER_H
#define CRAWLER_H

#include <QObject>
#include "SystemInfo.h"
class DetectIdle;
class Crawler : public QObject
{
    Q_OBJECT
    DetectIdle * _di;
public:
    explicit Crawler(QObject *parent = 0);
    QList<WindowInfo> all(int seconds_before);
signals:

public slots:
protected slots:
    void onTimer();
private:

};

#endif // LOGIC_H
