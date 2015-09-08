#include "SystemInfo.h"
bool operator==(WindowInfo _1, WindowInfo _2)
{
    if(_1.isSystemIsIdle != _2.isSystemIsIdle)
        return false;
    if(_1.windowClass != _2.windowClass)
        return false;
    if(_1.windowName != _2.windowName)
        return false;
    return true;
}
