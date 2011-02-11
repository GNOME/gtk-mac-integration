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

#ifndef __COCOA_MENU_ITEM_H__
#define __COCOA_MENU_ITEM_H__

#import <Cocoa/Cocoa.h>
#include <gtk/gtk.h>
#include "cocoa_menu.h"
#import "GNSMenuItem.h"

GNSMenuItem *cocoa_menu_item_get(GtkWidget* menu_item);

void cocoa_menu_item_add_item (NSMenu* cocoa_menu,
			       GtkWidget* menu_item,
			       int index);

void cocoa_menu_item_add_submenu (GtkMenuShell *menu_shell,
				  NSMenu*       cocoa_menu,
				  gboolean      toplevel,
				  gboolean      debug);


#endif __COCOA_MENU_ITEM_H__
