#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
#include <QString>
struct WindowInfo
{
    QString windowClass;
    QString windowName;
    bool isSystemIsIdle;

    void operator=(WindowInfo & o);
};
bool operator==(WindowInfo _1, WindowInfo _2);
WindowInfo getWindowInfo();

#endif // SYSTEMINFO_H
