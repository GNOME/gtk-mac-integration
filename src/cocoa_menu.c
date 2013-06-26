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

#include "cocoa_menu.h"

static GQuark cocoa_menu_quark = 0;

NSMenu *
cocoa_menu_get (GtkWidget *widget)
{
  /* If cocoa_menu_quark == 0 then cocoa_menu_connect hasn't been
     called yet and we therefore don't have an NSMenu* to return. */
  if (cocoa_menu_quark <= 0)
    return NULL;
  return (NSMenu*) g_object_get_qdata (G_OBJECT (widget), cocoa_menu_quark);
}

static void
cocoa_menu_free (gpointer *ptr)
{
  NSMenu* menu = (NSMenu*) ptr;
  [menu release];
}

void
cocoa_menu_connect (GtkWidget *menu,
                    NSMenu*    cocoa_menu)
{
  [cocoa_menu retain];

  if (cocoa_menu_quark == 0)
    cocoa_menu_quark = g_quark_from_static_string ("NSMenu");

  g_object_set_qdata_full (G_OBJECT (menu), cocoa_menu_quark,
                           cocoa_menu,
                           (GDestroyNotify) cocoa_menu_free);
}
