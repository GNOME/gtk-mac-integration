/* GTKOSXApplication Class
 *
 * Copyright © 2010 John Ralls
 *
 * An application class which mirrors the Cocoa NSApplication to
 * receive signals from the GtkApplicationDelegate class in
 * gtkosxapplication_quartz.m
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

#include "gtkosxapplication.h"
#include "gtkosxapplicationprivate.h"

//#define DEBUG(format, ...) g_printerr ("%s: " format, G_STRFUNC, ## __VA_ARGS__)
#define DEBUG(format, ...)


/** 
 * gtk_osxapplication_use_quartz_accelerators:
 * @self: The GtkOSXApplication pointer.
 *
 * Are we using Quartz or Gtk+ accelerator handling? 
 *
 * Returns: a gboolean
 */
gboolean
gtk_osxapplication_use_quartz_accelerators(GtkOSXApplication *self)
{
    return self->priv->use_quartz_accelerators;
}

/** 
 * gtk_osxapplication_set_use_quartz_accelerators:
 * @self: The GtkOSXApplication pointer.
 * @use_quartz_accelerators: Gboolean 
 *
 * Set quartz accelerator handling; TRUE (default) uses quartz; FALSE
 * uses Gtk+. Quartz accelerator handling is required for normal OSX
 * accelerators (e.g., command-q to quit) to work.
 */
void
gtk_osxapplication_set_use_quartz_accelerators(GtkOSXApplication *self,
					    gboolean use_quartz_accelerators)
{
    self->priv->use_quartz_accelerators = use_quartz_accelerators;
}

/*
 * gtk_type_osxapplication_attention_type_get_type:
 *
 * A public enum used to set the parameter for attention
 * requests. Exists soley to satisfy the PyGObject codegen system.
 */
GType
gtk_type_osxapplication_attention_type_get_type(void)
{
  //Bogus GType, but there's no good reason to register this; it's only an enum
  return 0;
}
