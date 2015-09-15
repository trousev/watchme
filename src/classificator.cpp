#include "classificator.h"
#include <QDebug>
Classificator::Classificator()
{
}


QString CItemList::getMatch(QString probe, QString defaultProbe)
{
    for(int i=0; i<count(); i++)
    {
        CItem it = at(i);
        if(it.isMatch(probe))
            return it.name();
    }
    return defaultProbe;
}

CItemList::CItemList(QDomElement e): QList() {
    QDomNodeList items = e.elementsByTagName("item");
    for(int i=0; i<items.count(); i++)
    {
        QDomElement e = items.at(i).toElement();
        if(e.isNull())
        {
            qDebug() << "Strange: element is NULL. What's up?";
            continue;
        }
        append(CItem(e));
    }
}

QDomElement CItemList::toDomElement(QDomDocument doc) {
    QDomElement ans = doc.createElement("classification");
    for(int i=0; i<count(); i++)
    {
        ans.appendChild(at(i).toDomElement(doc));
    }
    return ans;
}


void CReport::track(QString category, float time)
{
    if(category.contains("/"))
    {
        QStringList arr = category.split("/");
        category = arr.takeFirst();
        QString subcategory = arr.join("/");
        _children[category].track(subcategory, time);
    }
    _data[category] += time;
    _total += time;
}

QMap<QString, float> CReport::result() const {
    QMap<QString, float> ans = _data;
    for(int i=0; i<ans.keys().count(); i++)
    {
        QString key = ans.keys().at(i);
        ans[key] = (ans[key] / _total) * 100.0;
    }
    return ans;
}

CReport CReport::child(QString name) {
    return _children[name];
}

CReport::CReport() : _total(0) {}
