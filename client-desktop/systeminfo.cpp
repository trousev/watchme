#include "systeminfo.h"

int operator ==(WindowInfo a, WindowInfo b) {
    return a.windowClass == b.windowClass && a.windowName == b.windowName && a.windowTitle == b.windowTitle;
}

int operator !=(WindowInfo a, WindowInfo b) {
    return !(a == b);
}
