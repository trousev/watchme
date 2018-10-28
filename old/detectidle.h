#ifndef DEDECTIDLE_H
#define DEDECTIDLE_H

#include <QObject>
#include <QPoint>
#include <QDateTime>

class DetectIdle : public QObject
{
    Q_OBJECT
    int _timeout;
    QPoint _last_pos;
    QDateTime last_action_time;
public:
    explicit DetectIdle(int timeout, QObject *parent = 0);

signals:

public:
    bool isIdle();
    int secondsSinceLastAction();

private slots:
    void onTimer();
};

#endif // DEDECTIDLE_H
