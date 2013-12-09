#ifndef SYSTEMINFO_X11_CPP
#define SYSTEMINFO_X11_CPP

#include "SystemInfo.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXSTR 1000

Display *__windowinfo__display;
unsigned long __windowinfo__window;
unsigned char *__windowinfo__prop;

void check_status(int status, unsigned long window)
{
    if (status == BadWindow) {
        printf("window id # 0x%lx does not exists!", window);
        exit(1);
    }

    if (status != Success) {
        printf("XGetWindowProperty failed!");
        exit(2);
    }
}

unsigned char* get_string_property(char* property_name)
{
    Atom actual_type, filter_atom;
    int actual_format;
    int status;
    unsigned long nitems;
    unsigned long bytes_after;

    filter_atom = XInternAtom(__windowinfo__display, property_name, True);
    status = XGetWindowProperty(__windowinfo__display, __windowinfo__window, filter_atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &__windowinfo__prop);
    check_status(status, __windowinfo__window);
    return __windowinfo__prop;
}

unsigned long get_long_property(char* property_name)
{
    get_string_property(property_name);
    unsigned long long_property = __windowinfo__prop[0] + (__windowinfo__prop[1]<<8) + (__windowinfo__prop[2]<<16) + (__windowinfo__prop[3]<<24);
    free(__windowinfo__prop);
    return long_property;
}



WindowInfo getWindowInfo()
{
    WindowInfo wi;

    char *display_name = NULL;  // could be the value of $DISPLAY
    __windowinfo__display = XOpenDisplay(display_name);
    if (__windowinfo__display == NULL) {
        return wi;
    }
    int screen = XDefaultScreen(__windowinfo__display);
    __windowinfo__window = RootWindow(__windowinfo__display, screen);
    __windowinfo__window = get_long_property("_NET_ACTIVE_WINDOW");

    char* wclass = (char*)get_string_property("WM_CLASS");
    char* wname = (char*)get_string_property("_NET_WM_NAME");
    wi.windowClass = QString::fromLocal8Bit( wclass );
    wi.windowName = QString::fromLocal8Bit( wname );
    free(wclass);
    free(wname);
    wi.windowTitle = wi.windowName;
    //wi.windowTitle = QString::fromLocal8Bit( (char*)get_string_property("WM_NAME") );
    //printf("_NET_WM_PID: %lu\n", get_long_property("_NET_WM_PID"));
    //printf("WM_CLASS: %s\n", get_string_property("WM_CLASS"));
    //printf("_NET_WM_NAME: %s\n", get_string_property("_NET_WM_NAME"));

    XCloseDisplay (__windowinfo__display );
    return wi;
}

#endif // SYSTEMINFO_X11_CPP
