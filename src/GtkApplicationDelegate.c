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

#import "GtkApplicationDelegate.h"
#include <gtk/gtk.h>
#include "gtkosxapplication.h"

@implementation GtkApplicationDelegate
-(BOOL) application:(NSApplication*) theApplication openFile:(NSString*) file
{
  const gchar *utf8_path =  [file UTF8String];
  GtkOSXApplication *app = g_object_new(GTK_TYPE_OSX_APPLICATION, NULL);
  guint sig = g_signal_lookup("NSApplicationOpenFile", 
			      GTK_TYPE_OSX_APPLICATION);
  gboolean result = FALSE;
  if (sig)
      g_signal_emit(app, sig, 1, utf8_path, &result);
  g_object_unref(app);
  return result;
}


- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)sender
{
  GtkOSXApplication *app = g_object_new(GTK_TYPE_OSX_APPLICATION, NULL);
  guint sig = g_signal_lookup("NSApplicationBlockTermination", 
			      GTK_TYPE_OSX_APPLICATION);
  gboolean result = FALSE;
  if (sig)
      g_signal_emit(app, sig, 0, &result);

  g_object_unref(app);
  if (!result)
    return NSTerminateNow;
  else
    return NSTerminateLater;
}

extern NSMenu* gtk_osxapplication_dock_menu(GtkOSXApplication* app);

-(NSMenu *)applicationDockMenu: (NSApplication*) sender
{
    g_print("Dock requested a menu\n");
    GtkOSXApplication *app = g_object_new(GTK_TYPE_OSX_APPLICATION, NULL);
    return gtk_osxapplication_dock_menu(app);
}

@end
