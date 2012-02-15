/* GTK+ Integration with platform-specific application-wide features 
 * such as the OS X menubar and application delegate concepts.
 *
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
#import <Cocoa/Cocoa.h>
#include <gtk/gtk.h>
#include "gtkosxapplication.h"

@class _GNSMenuItem;

/** 
 * SECTION:_GNSMenuBar
 * @short_description: Wrapper class for NSMenubar
 * #title: _GNSMenuBar
 * @stability: Private
 *
 * Wrapper class around NSMenu providing an extra
 * parameter for stashing the App menu groups.
 */

@interface _GNSMenuBar : NSMenu
{
@private
  GList *app_menu_groups;
  GtkMenuBar *gtk_menubar;
  _GNSMenuItem *app_menu;
  _GNSMenuItem *window_menu;
  _GNSMenuItem *help_menu;
}

/** 
 * initWithTitle:
 * @title: Title string with which to initialize the menubar. Normally @"".
 *
 * Override the designated initializer 
 */
- (id) initWithTitle: (NSString*) title;

/**
 * initWithGtkMenuBar:
 * @menubar: A pointer to the menubar we're going to sync with
 *
 * Provide the initializer we actually want to use
 */
- (id) initWithGtkMenuBar: (GtkMenuBar*) menubar;

/** 
 * addGroup:
 *
 * Create a new GtkApplicationMenuGroup, add it to the list, and
 * return a pointer to it.
 */
- (GtkosxApplicationMenuGroup *) addGroup;

/**
 * app_menu_groups:
 *
 * Get a pointer to the current head of the app_menu_groups list
 */
- (GList *) app_menu_groups;

/**
 * resync:
 *
 *  Resynchronize ourself with our GtkMenuBar
 */
-(void) resync;

- (GtkMenuBar*) menuBar;
- (void) setAppMenu: (_GNSMenuItem*) menu_item;
- (_GNSMenuItem*) appMenu;
- (void) setWindowsMenu: (_GNSMenuItem*) menu_item;
- (_GNSMenuItem*) windowsMenu;
- (void) setHelpMenu: (_GNSMenuItem*) menu_item;
- (_GNSMenuItem*) helpMenu;

/**
 * dealloc:
 *
 * Destructor
 */
- (void) dealloc;
@end
