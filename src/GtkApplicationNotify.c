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

#import "GtkApplicationNotify.h"
#include <gtk/gtk.h>
#include "gtkosxapplication.h"

@implementation GtkApplicationNotificationObject
-(GtkApplicationNotificationObject*) init
{
  self = [ super init ];
  g_return_val_if_fail (self != NULL, NULL);
  [[NSNotificationCenter defaultCenter] addObserver: self
   selector: @selector (appDidBecomeActive: )
   name: NSApplicationDidBecomeActiveNotification
   object: NSApp];

  [[NSNotificationCenter defaultCenter] addObserver: self
   selector: @selector (appDidBecomeInactive: )
   name: NSApplicationWillResignActiveNotification
   object: NSApp];

  [[NSNotificationCenter defaultCenter] addObserver: self
   selector: @selector (appWillTerminate: )
   name: NSApplicationWillTerminateNotification
   object: NSApp];

  return self;
}

-(void) appDidBecomeActive: (NSNotification *)notification
{
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  guint sig = g_signal_lookup ("NSApplicationDidBecomeActive",
			       GTKOSX_TYPE_APPLICATION);
  if (sig)
    g_signal_emit (app, sig, 0);
  g_object_unref (app);
}

-(void) appDidBecomeInactive: (NSNotification *)notification
{
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  guint sig = g_signal_lookup ("NSApplicationWillResignActive",
			       GTKOSX_TYPE_APPLICATION);
  if (sig)
    g_signal_emit (app, sig, 0);
  g_object_unref (app);
}

-(void) appWillTerminate: (NSNotification *)notification
{
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  [[NSNotificationCenter defaultCenter] removeObserver: self];
  guint sig = g_signal_lookup ("NSApplicationWillTerminate",
			       GTKOSX_TYPE_APPLICATION);
  if (sig)
    g_signal_emit (app, sig, 0);
  g_object_unref (app);

}

@end
