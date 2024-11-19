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

#if MAC_OS_X_VERSION_MIN_REQUIRED < 101200
#define GDK_QUARTZ_ALTERNATE_KEY_MASK NSAlternateKeyMask
#define GDK_QUARTZ_COMMAND_KEY_MASK NSCommandKeyMask
#define GDK_QUARTZ_CONTROL_KEY_MASK NSControlKeyMask
#define GDK_QUARTZ_SHIFT_KEY_MASK NSShiftKeyMask
#define GDK_QUARTZ_KEY_DOWN NSKeyDown
#else
#define GDK_QUARTZ_ALTERNATE_KEY_MASK NSEventModifierFlagOption
#define GDK_QUARTZ_COMMAND_KEY_MASK NSEventModifierFlagCommand
#define GDK_QUARTZ_CONTROL_KEY_MASK NSEventModifierFlagControl
#define GDK_QUARTZ_SHIFT_KEY_MASK NSEventModifierFlagShift
#define GDK_QUARTZ_KEY_DOWN NSEventTypeKeyDown
#endif

#if MAC_OS_X_VERSION_MIN_REQUIRED < 101300
#define GDK_QUARTZ_MIXED_STATE NSMixedState
#define GDK_QUARTZ_ON_STATE NSOnState
#define GDK_QUARTZ_OFF_STATE NSOffState
#define GDK_QUARTZ_NUMERIC_PAD_KEY_MASK NSNumericPadKeyMask
#else
#define GDK_QUARTZ_MIXED_STATE NSControlStateValueMixed
#define GDK_QUARTZ_ON_STATE NSControlStateValueOn
#define GDK_QUARTZ_OFF_STATE NSControlStateValueOff
#define GDK_QUARTZ_NUMERIC_PAD_KEY_MASK NSEventModifierFlagNumericPad
#endif

_GNSMenuItem *cocoa_menu_item_get(GtkWidget* menu_item);

void cocoa_menu_item_add_item (NSMenu* cocoa_menu,
			       GtkWidget* menu_item,
			       int index);

void cocoa_menu_item_add_submenu (GtkMenuShell *menu_shell,
				  NSMenu*       cocoa_menu,
				  gboolean      toplevel,
				  gboolean      debug);


#endif /* __COCOA_MENU_ITEM_H__ */
