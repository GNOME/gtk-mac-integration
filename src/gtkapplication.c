/* GTKApplication Class
 *
 * Copyright © 2010 John Ralls
 *
 * An application class which mirrors the Cocoa NSApplication to
 * receive signals from the GtkApplicationDelegate class in
 * gtkapplication_quartz.m
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

#include "gtkapplication.h"
#include "gtkapplicationprivate.h"



/** Override this function to do something useful when your app gets
 *  or loses the focus.
 */
void 
gtk_application_activation_changed (GtkApplication *self, gboolean changed) 
{
  // if (changed)
  //    g_print("Application now active\n");
  //else
  //  g_print("Application now inactive\n");
}

/** Override this function to load the indicated file.
 */
void gtk_application_should_load (GtkApplication *self, const gchar *utf8_path)
{
    g_print("Should Load %s\n", utf8_path);
}

/** Override this function to check for open files or the like and to
 *  return FALSE if you're not ready to quit, or TRUE if you are.
 */

gboolean gtk_application_should_quit (GtkApplication *self)
{
  return TRUE;
}


/* Public GtkApplication Functions */

