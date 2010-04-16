/* GTK+ Integration with platform-specific application-wide features 
 * such as the OS X menubar and application delegate concepts.
 *
 * Copyright (C) 2009 Paul Davis
 * Copyright © 2010 John Ralls
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; version 2.1
 * of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#import <Cocoa/Cocoa.h>
#include <gtk/gtk.h>
// #include "gtkapplication.h"

typedef struct {
  GClosure *closure;
  gpointer data;
} ClosureData;


/**
 * GNSMenuItem is a wrapper class around NSMenuItem providing an
 * action function which puts invocation of the provided GClosure onto
 * the gtk idle queue. 
 */
@interface GNSMenuItem : NSMenuItem
{
@public
  /// action_closure is the closure invoked when the menu item is
  /// activated (usually by clicking on it).
  ClosureData action;
  //accel_closure is manipulated directly by
  //cocoa_menu_item_update_accel_closure()
  GClosure *accel_closure; 
  // The hidden parameter was introduced in 10.5; for earlier OSX
  // versions we need to emulate it.
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
  BOOL hidden;
  uint index;
  NSMenu *inMenu;
#endif

}

/** Create a new GNSMenuItem with a GClosure and an additional arbitrary data struct */

- (id) initWithTitle:(NSString*) title aGClosure:(GClosure*) closure andPointer:(gpointer) ptr;

/** overrides the superclass function and puts (indirectly) the
 *  action_closure on the idle queue.
 */

- (void) activate:(id) sender;

- (BOOL) isHidden;
- (void) setHidden: (BOOL) shouldHide;
@end
