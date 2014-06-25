#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QRegExp>
#include <QList>
#include "SystemInfo.h"
class Logic : public QObject
{
    Q_OBJECT
    struct Rule
    {
        QString category;
        QRegExp regex;
    };
    QList<Rule> rules;

public:
    explicit Logic(QObject *parent = 0);

signals:

public slots:
    void updateRules();
    QString resolve(WindowInfo wi);
};

#endif // LOGIC_H
