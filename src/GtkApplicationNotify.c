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
#include "gtkapplication.h"

@implementation GtkApplicationNotificationObject
- (GtkApplicationNotificationObject*) init
{
  self = [ super init ];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
					  selector:@selector(appDidBecomeActive:)
					  name:NSApplicationDidBecomeActiveNotification
					  object:[NSApplication sharedApplication]];

    [[NSNotificationCenter defaultCenter] addObserver:self
					  selector:@selector(appDidBecomeInactive:)
					  name:NSApplicationWillResignActiveNotification 
					  object:[NSApplication sharedApplication]];
  }

  return self;
}

- (void)appDidBecomeActive:(NSNotification *)notification
{
  GtkApplication *app = g_object_new(GTK_TYPE_APPLICATION, NULL);
  gtk_application_activation_changed(app, true);
  g_object_unref(app);
}

- (void)appDidBecomeInactive:(NSNotification *)notification
{
  GtkApplication *app = g_object_new(GTK_TYPE_APPLICATION, NULL);
  gtk_application_activation_changed(app, false);
  g_object_unref(app);
}

@end
