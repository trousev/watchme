#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
#include <QString>
struct WindowInfo
{
    QString windowClass;
    QString windowName;
    QString windowTitle;
};

int operator ==(WindowInfo a, WindowInfo b);
int operator !=(WindowInfo a, WindowInfo b);

WindowInfo getWindowInfo();

#endif // SYSTEMINFO_H
