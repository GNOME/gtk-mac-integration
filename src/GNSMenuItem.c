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
#import "GNSMenuItem.h"
#import "GNSMenuBar.h"

static gboolean
idle_call_activate (ClosureData *action)
{
//    g_value_init(&args, GTK_TYPE_MENU_ITEM);
  GValue arg = {0};
  g_value_init(&arg, G_TYPE_POINTER);
  g_value_set_pointer(&arg, action->data);
  g_closure_invoke(action->closure, NULL, 1, &arg, 0);
//  gtk_menu_item_activate ((GtkMenuItem*) data);
  return FALSE;
}

@implementation GNSMenuItem

- (id) initWithTitle:(NSString*) title aGClosure:(GClosure*) closure andPointer:(gpointer) ptr
{

  /* All menu items have the action "activate", which will be handled by this child class
   */
  self = [ super initWithTitle:title action:@selector(activate:) keyEquivalent:@"" ];

  if (self) {
    /* make this handle its own action */
    [ self setTarget:self ];
    g_closure_ref(closure);
    g_closure_sink(closure);
    action.closure = closure;
    action.data = ptr;
    accel_closure = NULL;
  }
  return self;
}

- (void) activate:(id) sender
{
    g_idle_add ((GSourceFunc)idle_call_activate, &action);
}

- (BOOL) isHidden
{
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4
  return [super isHidden];
#else
  return hidden;
#endif
}
  
- (void) setHidden: (BOOL) shouldHide
{
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4
  [super setHidden: shouldHide];
#else
  [self retain];
  if (!hidden && shouldHide) {
    inMenu = [self menu];
    index = [inMenu indexOfItem: self];
    [inMenu removeItem: self];
    hidden = YES;
  }
  else if (hidden && !shouldHide) {
    [inMenu insertItem: self atIndex: index];
    [(GNSMenuBar*)[NSApp mainMenu] resync];
    hidden = NO;
  }
  [self release];
#endif
}

@end
