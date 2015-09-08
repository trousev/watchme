#ifndef SYSTEMINFO_X11_CPP
#define SYSTEMINFO_X11_CPP

#include "SystemInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#define MAXSTR 1000
QString _do_repplace(QString in)
{
    QRegExp r(".*=");
    r.setMinimal(true);
    in = in.replace(r, "");
    in = in.replace("\n","");
    in = in.replace("\r","");
    in = in.trimmed();
    return in;
}

WindowInfo getWindowInfo()
{
    WindowInfo wi;
    FILE * p = popen("xprop -id $(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) WM_CLASS _NET_WM_NAME WM_WINDOW_ROLE", "r");
    char buf[5][MAXSTR];
    int i=0;
    while(!feof(p))
    {
        if(!fgets(buf[i], MAXSTR, p)) break;
        i++;
    }
    pclose(p);
    if(i < 2) return wi;
    wi.windowClass = buf[0];
    wi.windowName = buf[1];

    wi.windowClass = _do_repplace(wi.windowClass);
    wi.windowName = _do_repplace(wi.windowName);

    p = popen("xprintidle","r");
    int idle;
    fscanf(p, "%d", &idle);
    wi.isSystemIsIdle = (idle > 600000);
    return wi;
}

#endif // SYSTEMINFO_X11_CPP
