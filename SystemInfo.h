#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
#include <QString>
struct WindowInfo
{
    QString windowClass;
    QString windowName;
    QString windowTitle;
};

WindowInfo getWindowInfo();

#endif // SYSTEMINFO_H
