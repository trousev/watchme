#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>

class Crawler : public QObject
{
    Q_OBJECT
public:
    explicit Crawler(QObject *parent = 0);

signals:

public slots:
protected slots:
    void onTimer();
private:

};

#endif // LOGIC_H
