#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H

#include <QDomElement>
#include <QDomDocument>

#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
#include <QRegExp>
#include "fcml.h"
class CItem: public  QPair<QString, QRegExp> {
public:
    QString name() {return first; }
    QRegExp query() {return second; }
    bool isMatch(QString probe) {
        return probe.contains(query());
    }
    CItem(QDomElement e): QPair(QString(e.attribute("name")),QRegExp(e.attribute("query"))) {}
    CItem(QString name, QRegExp query): QPair(name, query) {}
    CItem(QString name, QString query): QPair(name, QRegExp(query)) {}
    QDomElement toDomElement(QDomDocument doc) const{
        QDomElement e = doc.createElement("item");
        e.setAttribute("name", first);
        e.setAttribute("query", second.pattern());
        return e;
    }
};

class CItemList: public QList<CItem> {
public:
    QString getMatch(QString probe, QString defaultProbe=QString());
    CItemList(QDomElement e);
    CItemList():QList(){}
    QDomElement toDomElement(QDomDocument doc);
};


class CReport {
    QMap<QString, float> _data;
    QMap<QString, CReport> _children;
    int _total;
public:
    CReport();
    void track(QString category, float time);
    QMap<QString, float> result() const;
    CReport child(QString name);
};

class Classificator
{
public:
    Classificator();
};

#endif // CLASSIFICATOR_H
