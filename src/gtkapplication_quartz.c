/* GTK+ application-level integration for the Mac OS X/Cocoa 
 *
 * Copyright (C) 2007 Pioneer Research Center USA, Inc.
 * Copyright (C) 2007 Imendio AB
 * Copyright (C) 2009 Paul Davis
 *
 * This is a reimplementation in Cocoa of the sync-menu.c concept
 * from Imendio, although without the "set quit menu" API since
 * a Cocoa app needs to handle termination anyway.
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

#include <gtk/gtk.h>

#import "GtkApplicationDelegate.h"
#import "GtkApplicationNotify.h"

#include "gtkapplication.h"
#include "gtkapplicationprivate.h"
#include "cocoa_menu_item.h"
#include "cocoa_menu.h"
#include "getlabel.h"

#define DEBUG(format, ...) g_printerr ("%s: " format, G_STRFUNC, ## __VA_ARGS__)
//#define DEBUG(format, ...)

/* TODO
 *
 * - Sync adding/removing/reordering items
 * - Create on demand? (can this be done with gtk+? ie fill in menu
 items when the menu is opened)
 * - Figure out what to do per app/window...
 *
 */
G_DEFINE_TYPE (GtkApplication, gtk_application, G_TYPE_OBJECT)
/**
   @fn gboolean gdk_quartz_in_menu_event_handler (GtkApplication *self)
   @brief test whether GtkApplication is handling a Mac event.

   gtk/osx has a problem in that mac main menu events
   are handled using an "internal" event handling system that 
   doesn't pass things back to the glib/gtk main loop. if we call
   gtk_main_iteration() block while in a menu event handler, then
   glib gets confused and thinks there are two threads running
   g_main_poll_func(). apps call call gdk_quartz_in_menu_event_handler()
   if they need to check this.

   @param GtkApplication *self The application singleton item.
   @return gboolean TRUE if in a Mac event handler
*/



static gulong emission_hook_id = 0;

static gboolean
parent_set_emission_hook (GSignalInvocationHint *ihint,
			  guint                  n_param_values,
			  const GValue          *param_values,
			  gpointer               data)
{
  GtkWidget *instance = (GtkWidget*) g_value_get_object (param_values);

  if (GTK_IS_MENU_ITEM (instance))
    {
      GtkWidget *previous_parent = (GtkWidget*) g_value_get_object (param_values + 1);
      GtkWidget *menu_shell      = NULL;

      if (GTK_IS_MENU_SHELL (previous_parent))
	{
	  menu_shell = previous_parent;
	}
      else if (GTK_IS_MENU_SHELL (instance->parent))
	{
	  menu_shell = instance->parent;
	}

      if (menu_shell)
	{
	  NSMenu *cocoa_menu = cocoa_menu_get (menu_shell);

	  if (cocoa_menu)
	    {
	      cocoa_menu_item_add_submenu (GTK_MENU_SHELL (menu_shell),
					 cocoa_menu,
					 cocoa_menu == (NSMenu*) data,
					 FALSE);
	    }
	}
    }

  return TRUE;
}

static void
parent_set_emission_hook_remove (GtkWidget *widget,
				 gpointer   data)
{
  g_signal_remove_emission_hook (g_signal_lookup ("parent-set",
						  GTK_TYPE_WIDGET),
				 emission_hook_id);
}


#warning You can safely ignore the next warning about a duplicate interface definition
@interface NSApplication(NSWindowsMenu)
- (void)setAppleMenu:(NSMenu *)aMenu;
@end


static int
add_to_menubar (GtkApplication *self, NSMenu *menu)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  [dummyItem setSubmenu:menu];
  [self->priv->main_menubar addItem:dummyItem];
  [dummyItem release];
  return 0;
}

static int
add_to_app_menu (GtkApplication *self, NSMenu *menu)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  [dummyItem setSubmenu:menu];
  [self->priv->app_menu addItem:dummyItem];
  [dummyItem release];
  return 0;
}

static int
add_to_window_menu (GtkApplication *self, NSMenu *menu)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  [dummyItem setSubmenu:menu];
  [self->priv->window_menu addItem:dummyItem];
  [dummyItem release];
  return 0;
}

static int
create_apple_menu (GtkApplication *self)
{
  NSMenuItem *menuitem;
  // Create the application (Apple) menu.
  self->priv->app_menu = [[NSMenu alloc] initWithTitle: @"Apple Menu"];

  NSMenu *menuServices = [[NSMenu alloc] initWithTitle: @"Services"];
  [NSApp setServicesMenu:menuServices];

  [self->priv->app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle: @"Services"
				 action:nil keyEquivalent:@""];
  [menuitem setSubmenu:menuServices];
  [self->priv->app_menu addItem: menuitem];
  [menuitem release];
  [self->priv->app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide"
				 action:@selector(hide:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [self->priv->app_menu addItem: menuitem];
  [menuitem release];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide Others"
				 action:@selector(hideOtherApplications:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [self->priv->app_menu addItem: menuitem];
  [menuitem release];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Show All"
				 action:@selector(unhideAllApplications:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [self->priv->app_menu addItem: menuitem];
  [menuitem release];
  [self->priv->app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Quit"
				 action:@selector(terminate:) keyEquivalent:@"q"];
  [menuitem setTarget: NSApp];
  [self->priv->app_menu addItem: menuitem];
  [menuitem release];

  [NSApp setAppleMenu:self->priv->app_menu];
  add_to_menubar (self, self->priv->app_menu);

  return 0;
}

static int
create_window_menu (GtkApplication *self)
{   
  self->priv->window_menu = [[NSMenu alloc] initWithTitle: @"Window"];

  [self->priv->window_menu addItemWithTitle:@"Minimize"
		action:@selector(performMiniaturize:) keyEquivalent:@""];
  [self->priv->window_menu addItem: [NSMenuItem separatorItem]];
  [self->priv->window_menu addItemWithTitle:@"Bring All to Front"
		action:@selector(arrangeInFront:) keyEquivalent:@""];

  [NSApp setWindowsMenu:self->priv->window_menu];
  add_to_menubar(self, self->priv->window_menu);

  return 0;
}  


static GObject *
gtk_application_constructor (GType gtype,
			     guint n_properties,
			     GObjectConstructParam *properties)
{
  static GObject *self = NULL;
  //static GOnce once = G_ONCE_INIT;
  if (self == NULL)
    {
/*       struct construction_args args; */
/*       args.gtype = gtype; */
/*       args.n_props = n_properties; */
/*       args.props = properties; */
/*       g_once (&once, _parent_constructor, &args); */
/*       self = G_OBJECT_CLASS (once.retval); */
      self = G_OBJECT_CLASS(gtk_application_parent_class)->constructor(gtype, n_properties, properties);
      g_object_add_weak_pointer (self, (gpointer) &self);
      return self;
    }

  return g_object_ref (self);

}

struct construction_args {
  GType gtype;
  guint n_props;
  GObjectConstructParam *props;
};

static void
gtk_application_init (GtkApplication *self)
{
  [NSApplication sharedApplication];
  self->priv = GTK_APPLICATION_GET_PRIVATE (self);
  self->priv->main_menubar = [[NSMenu alloc] initWithTitle: @""];
  self->priv->in_menu_event_handler = FALSE;

  g_return_if_fail (self->priv->main_menubar != NULL);

  [NSApp setMainMenu: self->priv->main_menubar];
  create_apple_menu (self);
  // create_window_menu (self);

  /* this will stick around for ever ... is that OK ? */

  [ [GtkApplicationNotificationObject alloc] init];
  [ NSApp setDelegate: [GtkApplicationDelegate new]];
}


void 
gtk_application_class_init(GtkApplicationClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  g_type_class_add_private(klass, sizeof(GtkApplicationPrivate));
  gobject_class->constructor = gtk_application_constructor;
}

void
gtk_application_ready (GtkApplication *self)
{
  [ NSApp finishLaunching ];
}

void
gtk_application_cleanup(GtkApplication *self)
{
    GList *list;
  if (self->priv->window_menu)
    [ self->priv->window_menu release ];
  if (self->priv->app_menu)
    [ self->priv->app_menu release ];
  if (self->priv->main_menubar)
    [ self->priv->main_menubar release ];
  
  for (list = self->priv->menu_groups; list; list = g_list_next(list)) {
    if (list->data)
      g_list_free(list->data);
  }
  g_list_free(self->priv->menu_groups);
}

void
gtk_application_set_menu_bar (GtkApplication *self, GtkMenuShell *menu_shell)
{
  NSMenu* cocoa_menubar;

  g_return_if_fail (GTK_IS_MENU_SHELL (menu_shell));

  cocoa_menubar = [NSApp mainMenu];

  /* turn off auto-enabling for the menu - its silly and slow and
     doesn't really make sense for a Gtk/Cocoa hybrid menu.
  */

  [cocoa_menubar setAutoenablesItems:NO];

  emission_hook_id =
    g_signal_add_emission_hook (g_signal_lookup ("parent-set",
						 GTK_TYPE_WIDGET),
				0,
				parent_set_emission_hook,
				cocoa_menubar, NULL);


  g_signal_connect (menu_shell, "destroy",
		    G_CALLBACK (parent_set_emission_hook_remove),
		    NULL);

  cocoa_menu_item_add_submenu (menu_shell, cocoa_menubar, TRUE, FALSE);
}

void
gtk_application_add_app_menu_item (GtkApplication *self,
				   GtkApplicationMenuGroup *group,
				   const gchar *label,
				   GClosure *menu_action,
				   gpointer action_data)
{
  // we know that the application menu is always the submenu of the first item in the main menu
  GList   *list;
  gint     index = 0;


  g_return_if_fail (group != NULL);

  for (list = self->priv->menu_groups; list; list = g_list_next (list))
    {
      GtkApplicationMenuGroup *list_group = (GtkApplicationMenuGroup*) list->data;

      index += g_list_length (list_group->items);

      /*  adjust index for the separator between groups, but not
       *  before the first group
       */
      if (list_group->items && list->prev)
	index++;

      if (group == list_group)
	{
	  /*  add a separator before adding the first item, but not
	   *  for the first group
	   */
		
	  if (!group->items && list->prev)
	    {
	      [self->priv->app_menu insertItem:[NSMenuItem separatorItem] 
	       atIndex:index+1];
	      index++;
	    }
	  DEBUG ("Add to APP menu bar %s\n", label);
	  cocoa_menu_item_add_action (self->priv->app_menu, label, 
				      menu_action, action_data, index+1);

	  group->items = g_list_append (group->items, menu_action);
	  return;
	}
    }

  if (!list)
    g_warning ("%s: app menu group %p does not exist",
	       G_STRFUNC, group);
}


gboolean 
gdk_quartz_in_menu_event_handler (GtkApplication *self)
{
  return self->priv->in_menu_event_handler;
}
