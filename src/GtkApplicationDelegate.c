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
#include "gtkapplication.h"

@implementation GtkApplicationDelegate
-(BOOL) application:(NSApplication*) theApplication openFile:(NSString*) file
{
  const gchar *utf8_path =  [file UTF8String];
  GtkApplication *app = g_object_new(GTK_TYPE_APPLICATION, NULL);
  gtk_application_should_load(app, utf8_path);
  g_object_unref(app);
  return 1;
}


- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)sender
{
  GtkApplication *app = g_object_new(GTK_TYPE_APPLICATION, NULL);
  gboolean result = gtk_application_should_quit(app);
  g_object_unref(app);
  if (result)
    return NSTerminateNow;
  else
    return NSTerminateLater;
}


-(NSMenu *)applicationDockMenu: (NSApplication*) sender
{
    extern NSMenu* gtk_application_dock_menu(GtkApplication* app);
    g_print("Dock requested a menu");
    GtkApplication *app = g_object_new(GTK_TYPE_APPLICATION, NULL);
    return gtk_application_dock_menu(app);
}

@end
