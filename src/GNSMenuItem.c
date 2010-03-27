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

static gboolean
idle_call_activate (GClosure *closure)
{
    GValue args = {0};
    g_value_init(&args, GTK_TYPE_MENU_ITEM);
    g_value_set_instance(&args, closure->data);
    g_closure_invoke(closure, NULL, 1, &args, 0);
//  gtk_menu_item_activate ((GtkMenuItem*) data);
  return FALSE;
}

@implementation GNSMenuItem
- (id) initWithTitle:(NSString*) title andGClosure:(GClosure*) closure
{
  /* All menu items have the action "activate", which will be handled by this child class
   */

  self = [ super initWithTitle:title action:@selector(activate:) keyEquivalent:@"" ];

  if (self) {
    /* make this handle its own action */
    [ self setTarget:self ];
    g_closure_ref(closure);
    g_closure_sink(closure);
    action_closure = closure;
    accel_closure = 0;
  }
  return self;
}
- (void) activate:(id) sender
{
    g_idle_add ((GSourceFunc)idle_call_activate, action_closure);
}
@end
