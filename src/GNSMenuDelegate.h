#import <Cocoa/Cocoa.h>
#include <gtk/gtk.h>

#if (MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5)
@interface _GNSMenuDelegate : NSObject <NSMenuDelegate> {}
#else
@interface _GNSMenuDelegate : NSObject {}
#endif
@end
