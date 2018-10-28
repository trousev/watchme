#include "detectidle.h"
#include <QTimer>
#include <QCursor>
#include <QDateTime>
DetectIdle::DetectIdle(int timeout, QObject *parent):
    _timeout(timeout), QObject(parent)
{
    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    last_action_time = QDateTime::currentDateTime();
}


bool DetectIdle::isIdle()
{
    return secondsSinceLastAction() > _timeout;
}

int DetectIdle::secondsSinceLastAction()
{
    return last_action_time.secsTo(QDateTime::currentDateTime());
}

void DetectIdle::onTimer()
{
    QPoint pos = QCursor::pos();
    if( pos != _last_pos)
        last_action_time = QDateTime::currentDateTime();
    _last_pos = pos;
}
