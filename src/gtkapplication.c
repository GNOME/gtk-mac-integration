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

//#define DEBUG(format, ...) g_printerr ("%s: " format, G_STRFUNC, ## __VA_ARGS__)
#define DEBUG(format, ...)


/** Override this function to do something useful when your app gets
 *  or loses the focus.
 */
void 
gtk_application_activation_changed (GtkApplication *self, gboolean changed) 
{
  if (changed)
    DEBUG("Application now active\n");
  else
    DEBUG("Application now inactive\n");
}
/** Are we using Quartz or Gtk+ accelerator handling? */
gboolean
gtk_application_use_quartz_accelerators(GtkApplication *self)
{
    return self->priv->use_quartz_accelerators;
}

/** Set quartz accelerator handling; TRUE (default) uses quartz; FALSE
 * uses Gtk+. Quartz accelerator handling is required for normal OSX
 * accelerators (e.g., command-q to quit) to work. */
void
gtk_application_set_use_quartz_accelerators(GtkApplication *self,
					    gboolean use_quartz_accelerators)
{
    self->priv->use_quartz_accelerators = use_quartz_accelerators;
}

/** Override this function to load the indicated file.
 */
void gtk_application_should_load (GtkApplication *self, const gchar *utf8_path)
{
  DEBUG("Should Load %s\n", utf8_path);
}

/** Override this function to check for open files or the like and to
 *  return FALSE if you're not ready to quit, or TRUE if you are.
 */

gboolean gtk_application_should_quit (GtkApplication *self)
{
  DEBUG("Application Should Quit\n");
  return TRUE;
}


/* Public GtkApplication Functions */

