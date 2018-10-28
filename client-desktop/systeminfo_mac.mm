#include "SystemInfo.h"
#include <QDebug>
#import "Cocoa/Cocoa.h"
WindowInfo getWindowInfo()
{
    WindowInfo ans;
    int i =0;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSMutableArray *windows = (NSMutableArray *)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);

    for (NSDictionary *window in windows) {
        NSString *owner = [window objectForKey:@"kCGWindowOwnerName" ];
        NSString *name = [window objectForKey:@"kCGWindowName" ];
        NSString *layer = [window objectForKey:@"kCGWindowLayer"];
        if([layer intValue] == 0 && i == 0){
             NSString *title =  [owner stringByAppendingString:@" - "];
             //title = [title stringByAppendingString:name];
             QString foregroundAppTitle = QString::fromNSString(title);
             qDebug() << "WIII" << foregroundAppTitle;
             ans.windowClass = QString::fromNSString(title);
             ans.windowName = QString::fromNSString(name);
             ans.windowTitle = ans.windowName;
             i++;
        }
    }
    return ans;
}
