#include "logic.h"
#include <QFile>
#include <QDir>
#include <QDebug>
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    updateRules();
}

void Logic::updateRules()
{
    rules.clear();
    QFile f(QDir::home().absoluteFilePath(".watchme.rules"));
    if(!f.open(QFile::ReadOnly))
        return ;
    while(true)
    {
        QString data = QString::fromUtf8(f.readLine());
        data = data.trimmed();
        if(f.atEnd() && data.isEmpty()) break;
        QStringList l = data.split(":");
        if(l.size() < 2) continue;
        data = data.right(data.length() - l[0].length()-1);

        Rule r;
        r.category = l[0];
        r.regex = QRegExp(data);
        rules << r;
    }
}

QString Logic::resolve(WindowInfo wi)
{
    QString match = QString("class='%1' title='%2' name='%3'").arg(wi.windowClass, wi.windowTitle, wi.windowName);
    foreach(Rule r, rules)
    {
        if(r.regex.indexIn(match) >= 0)
        {
            return r.category;
        }
    }
    qDebug() << match;
    return "[Something Else]";
}
