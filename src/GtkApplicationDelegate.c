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
#include <gmodule.h>
#include "gtkosxapplication.h"

#if MAC_OS_X_VERSION_MIN_REQUIRED >= 10130
static GSList *utf8_paths = NULL;
#else
static gchar *utf8_path = NULL;
#endif
@implementation GtkApplicationDelegate

-(void) applicationDidFinishLaunching: (NSNotification*)aNotification
{
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 10130
    if (utf8_paths != NULL)
    {
      guint sig = g_signal_lookup ("NSApplicationOpenFile",
                                   GTKOSX_TYPE_APPLICATION);
      if (sig)
      {
        GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
        gboolean result = TRUE;
        g_signal_emit (app, sig, 0, utf8_paths, &result);
        if (result == TRUE)
        {
          [app replyToOpenOrPrint:NSApplicationDelegateReplySuccess];
        }
        else
        {
          [app replyToOpenOrPrint:NSApplicationDelegateReplyFailure];
        }
        g_slist_free_full(utf8_paths, g_free);
        utf8_paths = NULL;
        g_object_unref (app);
      }
    }
#else
    if (utf8_path != NULL)
    {
      guint sig = g_signal_lookup ("NSApplicationOpenFile",
                                   GTKOSX_TYPE_APPLICATION);
      if (sig)
      {
        gboolean result = FALSE;
        GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
        g_signal_emit (app, sig, 0, utf8_path, &result);
        g_free(utf8_path);
        utf8_path = NULL;
        g_object_unref (app);
      }
    }
#endif
}

#if MAC_OS_X_VERSION_MIN_REQUIRED >= 10130
-(void) application: (NSApplication*)theApplication openURLs: (NSArray<NSURL *> *) urls
{
  gboolean overall_result = TRUE;
  guint sig = g_signal_lookup ("NSApplicationOpenFile",
                               GTKOSX_TYPE_APPLICATION);
  if (sig)
  {
    GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
      for (NSURL* url in urls)
      {
        gboolean result = TRUE;
        utf8_paths = g_slist_append(utf8_paths, g_strdup([[url absoluteString] UTF8String]));
        g_signal_emit (app, sig, 0, [[url absoluteString] UTF8String], &result);
        if (!result)
          overall_result = FALSE;
      }
      [theApplication replyToOpenOrPrint: overall_result ? NSApplicationDelegateReplySuccess : NSApplicationDelegateReplyFailure];
    g_slist_free_full(utf8_paths, g_free);
    utf8_paths = NULL;
    g_object_unref (app);
  }
}
#else
-(BOOL) application: (NSApplication*)theApplication openFile: (NSString*) file
{
  utf8_path =  g_strdup([file UTF8String]);
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  guint sig = g_signal_lookup ("NSApplicationOpenFile",
                               GTKOSX_TYPE_APPLICATION);
  gboolean result = FALSE;
  if (sig)
    {
      g_signal_emit (app, sig, 0, utf8_path, &result);
      g_free(utf8_path);
      utf8_path = NULL;
    }
  g_object_unref (app);
  return result;
}
#endif

-(NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *)sender
{
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  guint sig = g_signal_lookup ("NSApplicationBlockTermination",
  GTKOSX_TYPE_APPLICATION);
  gboolean result = FALSE;
  static gboolean inHandler = FALSE;
  if (inHandler) return NSTerminateCancel;
  if (sig)
    {
      inHandler = TRUE;
      g_signal_emit (app, sig, 0, &result);
    }

  g_object_unref (app);
  inHandler = FALSE;
  if (!result)
    {
      g_object_unref (app);
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 10130
      g_slist_free_full(utf8_paths, g_free);
#else
      g_free (utf8_path);
#endif
      return NSTerminateNow;
    }
  else
    return NSTerminateCancel;
}

extern NSMenu* _gtkosx_application_dock_menu (GtkosxApplication* app);

-(NSMenu *) applicationDockMenu: (NSApplication*)sender
{
  GtkosxApplication *app = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  return _gtkosx_application_dock_menu (app);
}

@end
