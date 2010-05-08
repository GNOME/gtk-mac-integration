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
#import "GNSMenuBar.h"
#include "cocoa_menu_item.h"

@implementation GNSMenuBar

- (id) initWithTitle:(NSString*) title
{
  self = [super initWithTitle: title];
  app_menu_groups = nil;
  return self;
}

- (id) initWithGtkMenuBar: (GtkMenuBar*) menubar
{
  self = [self initWithTitle: @""];
  gtk_menubar = menubar;
  return self;
}

- (GtkOSXApplicationMenuGroup*) addGroup
{
  GtkOSXApplicationMenuGroup *group = g_slice_new0 (GtkOSXApplicationMenuGroup);
  app_menu_groups = g_list_append (app_menu_groups, group);
  return group;
}

- (GList *) app_menu_groups
{
  return app_menu_groups;
}

- (void) resync
{
  cocoa_menu_item_add_submenu(GTK_MENU_SHELL(gtk_menubar), self, TRUE, FALSE);
    if (help_menu && [self indexOfItem: (NSMenuItem*)help_menu] < [self numberOfItems] - 1) {
    [self removeItem: (NSMenuItem*)help_menu];
    [self addItem: (NSMenuItem*)help_menu];
  }
  if ([self indexOfItem: (NSMenuItem*)window_menu] != [self numberOfItems] - 2) {
    [self removeItem: (NSMenuItem*)window_menu];
    [self insertItem: (NSMenuItem*)window_menu atIndex: [self numberOfItems] - 1];
    }
}

- (void) setAppMenu: (GNSMenuItem*) menu_item
{
  app_menu = menu_item;
}

- (GNSMenuItem*) appMenu {
  return app_menu;
}

- (void) setWindowMenu: (GNSMenuItem*) menu_item 
{
  window_menu = menu_item;
}

- (GNSMenuItem*) windowMenu
{
  return window_menu;
}

- (void) setHelpMenu: (GNSMenuItem*) menu_item
{
  help_menu = menu_item;
}

- (GNSMenuItem*) helpMenu
{
  return help_menu;
}

- (void) dealloc
{
  GList *list;
  for (list = app_menu_groups; list; list = g_list_next(list)) {
    if (list && list->data)
      g_list_free(list->data);
  }
  if (app_menu_groups)
    g_list_free(app_menu_groups);
  [super dealloc];

}
@end
