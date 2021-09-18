/* --- objc-mode --- */
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
  g_value_init (&arg, G_TYPE_POINTER);
  g_value_set_pointer (&arg, action->data);
  g_closure_invoke (action->closure, NULL, 1, &arg, 0);
//  gtk_menu_item_activate ((GtkMenuItem*) data);
  return FALSE;
}

@implementation _GNSMenuItem

-(id) initWithTitle: (NSString*) title
           aGClosure: (GClosure*) closure
	   andPointer: (gpointer) gtkmenuitem
{

  /* All menu items have the action "activate", which will be handled by this child class
   */
  const char* locale = setlocale(LC_NUMERIC, "C");
  self = [ super initWithTitle: title action: @selector (activate: ) keyEquivalent: @"" ];

  if (self)
    {
      /* make this handle its own action */
      [ self setTarget: self ];
      g_closure_ref (closure);
      g_closure_sink (closure);
      action.closure = closure;
      action.data = NULL;
      accel_closure = NULL;
      notUsed = NO;
      g_weak_ref_set (&menuItem, GTK_MENU_ITEM (gtkmenuitem));
    }
  setlocale(LC_NUMERIC, locale);
  return self;
}

-(void) activate: (id) sender
{
  /* Add an idle in a thread-safe way and wake up the main loop with
     a bogus event: */
  GdkEvent *wake_up = gdk_event_new (GDK_NOTHING);
  gdk_threads_add_idle ((GSourceFunc)idle_call_activate, &action);
  gdk_event_put (wake_up);
  gdk_event_free (wake_up);
}

-(BOOL) isHidden
{
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4
  return [super isHidden];
#else
  return hidden;
#endif
}

-(void) setHidden: (BOOL)shouldHide
{
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4
  [super setHidden: shouldHide];
#else
  [self retain];
  if (!hidden && shouldHide)
    {
      inMenu = [self menu];
      index = [inMenu indexOfItem: self];
      [inMenu removeItem: self];
      hidden = YES;
    }
  else if (hidden && !shouldHide)
    {
      int maxIndex = [inMenu numberOfItems];
      hidden = NO;
      if (index < 0) index = 0;
      if (index > maxIndex) index = maxIndex;
      [inMenu insertItem: self atIndex: index];
      [(_GNSMenuBar*)[NSApp mainMenu] resync];
    }
  [self release];
#endif
}

-(BOOL) isEnabled
{
  return [self action] != nil;
}

-(void) setEnabled: (BOOL)shouldEnable
{
  if (shouldEnable)
    [self setAction: @selector (activate:)];
  else
    [self setAction: nil];
}

-(void) mark
{
  notUsed = YES;
}

-(void) unmark
{
  notUsed = NO;
}

-(BOOL) isMarked
{
  return notUsed;
}

-(void) removeFromMenu: (NSMenu*)old_menu
{
#if !(MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4)
  if (old_menu != inMenu && old_menu != [self menu])
    return;
  [[self menu] removeItem: self];
  inMenu = nil;
  index = -1;
#else
  if (old_menu == [self menu])
    [[self menu] removeItem: self];
#endif
}

-(void) willHighlight
{
  NSMenu *theMenu = [self menu];
  guint index = [theMenu indexOfItem: self];
  GtkMenuItem *item = g_weak_ref_get (&menuItem);
  if (item != NULL)
    {
      GtkMenu *menu = GTK_MENU (gtk_widget_get_parent (GTK_WIDGET (item)));
      gtk_menu_set_active (menu, index);
      g_object_unref (item);
    }
}
@end
