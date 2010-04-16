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
#import "GNSMenuBar.h"

#include "gtkapplication.h"
#include "gtkapplicationprivate.h"
#include "cocoa_menu_item.h"
#include "cocoa_menu.h"
#include "getlabel.h"


/* This is a private function in libgdk; we need to have is so that we
   can force new windows onto the Window menu */
extern NSWindow* gdk_quartz_window_get_nswindow(GdkWindow*);

//#define DEBUG(format, ...) g_printerr ("%s: " format, G_STRFUNC, ## __VA_ARGS__)
#define DEBUG(format, ...)

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

/** Add a submenu to the currently active main menubar.
 */
static int
add_to_menubar (GtkApplication *self, NSMenu *menu, NSInteger pos)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  NSMenu *menubar = [NSApp mainMenu];

  [dummyItem setSubmenu:menu];
  if (pos < 0)
      [menubar addItem:dummyItem];
  else
      [menubar insertItem:dummyItem atIndex:pos];
  [dummyItem release];
  return 0;
}

/* Not Used
static int
add_to_app_menu (GtkApplication *self, NSMenu *menu)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  [dummyItem setSubmenu:menu];
  [[[[NSApp mainMenu] itemAtIndex: 0] submenu] addItem:dummyItem];
  [dummyItem release];
  return 0;
}
*/
 /* Not used
static int
add_to_window_menu (GtkApplication *self, NSMenu *menu)
{
  NSMenuItem *dummyItem = [[NSMenuItem alloc] initWithTitle:@""
					      action:nil keyEquivalent:@""];
  [dummyItem setSubmenu:menu];
  [[NSApp windowsMenu] addItem:dummyItem];
  [dummyItem release];
  return 0;
}
 */
static int
create_apple_menu (GtkApplication *self)
{
  NSMenuItem *menuitem;
  // Create the application (Apple) menu.
  NSMenu *app_menu = [[NSMenu alloc] initWithTitle: @"Apple Menu"];

  NSMenu *menuServices = [[NSMenu alloc] initWithTitle: @"Services"];
  [NSApp setServicesMenu:menuServices];

  [app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle: @"Services"
				 action:nil keyEquivalent:@""];
  [menuitem setSubmenu:menuServices];
  [app_menu addItem: menuitem];
  [menuitem release];
  [app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide"
				 action:@selector(hide:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [app_menu addItem: menuitem];
  [menuitem release];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Hide Others"
				 action:@selector(hideOtherApplications:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [app_menu addItem: menuitem];
  [menuitem release];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Show All"
				 action:@selector(unhideAllApplications:) keyEquivalent:@""];
  [menuitem setTarget: NSApp];
  [app_menu addItem: menuitem];
  [menuitem release];
  [app_menu addItem: [NSMenuItem separatorItem]];
  menuitem = [[NSMenuItem alloc] initWithTitle:@"Quit"
				 action:@selector(terminate:) keyEquivalent:@"q"];
  [menuitem setTarget: NSApp];
  [app_menu addItem: menuitem];
  [menuitem release];

  [NSApp performSelector:@selector(setAppleMenu:) withObject:app_menu];
  add_to_menubar (self, app_menu, 0);

  return 0;
}

static int
create_window_menu (GtkApplication *self, NSWindow* window)
{   
  NSMenu *window_menu = [[NSMenu alloc] initWithTitle: @"Window"];
  NSInteger pos;
  
  [window_menu addItemWithTitle:@"Minimize"
		action:@selector(performMiniaturize:) keyEquivalent:@""];
  [window_menu addItem: [NSMenuItem separatorItem]];
  [window_menu addItemWithTitle:@"Bring All to Front"
		action:@selector(arrangeInFront:) keyEquivalent:@""];

  [NSApp setWindowsMenu:window_menu];
  [NSApp addWindowsItem: window title: [window title] filename: NO];
  pos = [[NSApp mainMenu] indexOfItemWithTitle: @"Help"];
  add_to_menubar (self, window_menu, pos);

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

static GdkFilterReturn
global_event_filter_func (gpointer  windowing_event, GdkEvent *event,
                          gpointer  user_data)
{
  NSEvent *nsevent = windowing_event;
  GtkApplication* app = user_data;

  /* Handle menu events with no window, since they won't go through the
   * regular event processing.
   */
  if ([nsevent type] == NSKeyDown && 
      gtk_application_use_quartz_accelerators(app) )
    if ([[NSApp mainMenu] performKeyEquivalent: nsevent])
      return GDK_FILTER_TRANSLATE;
  return GDK_FILTER_CONTINUE;
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
  self->priv->use_quartz_accelerators = TRUE;
  gdk_window_add_filter (NULL, global_event_filter_func, (gpointer)self);

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
  //FIXME: release each window's menubar
  
}

static gboolean
window_focus_cb (GtkWindow* window, GdkEventFocus *event, GNSMenuBar *menubar)
{
  [NSApp setMainMenu: menubar];
  return FALSE;
}

void
gtk_application_set_menu_bar (GtkApplication *self, GtkMenuShell *menu_shell)
{
  GNSMenuBar* cocoa_menubar;
  GtkWidget *parent = gtk_widget_get_toplevel(GTK_WIDGET(menu_shell));
  GdkWindow *win = gtk_widget_get_window(parent);
  NSWindow *nswin; 

  g_return_if_fail (GTK_IS_MENU_SHELL (menu_shell));
  g_return_if_fail (win != NULL);
  g_return_if_fail (GDK_IS_WINDOW(win));
  nswin = gdk_quartz_window_get_nswindow(win);
  g_return_if_fail(nswin != NULL);

  cocoa_menubar = (GNSMenuBar*)cocoa_menu_get(GTK_WIDGET (menu_shell));
  if (!cocoa_menubar) {
    cocoa_menubar = [[GNSMenuBar alloc] initWithTitle: @""];
    cocoa_menu_connect(GTK_WIDGET (menu_shell), cocoa_menubar);
  /* turn off auto-enabling for the menu - its silly and slow and
     doesn't really make sense for a Gtk/Cocoa hybrid menu.
  */
    [cocoa_menubar setAutoenablesItems:NO];

  }
  if (cocoa_menubar != [NSApp mainMenu])
    [NSApp setMainMenu: cocoa_menubar];

  create_apple_menu (self);

  emission_hook_id =
    g_signal_add_emission_hook (g_signal_lookup ("parent-set",
						 GTK_TYPE_WIDGET),
				0,
				parent_set_emission_hook,
				cocoa_menubar, NULL);


  g_signal_connect (menu_shell, "destroy",
		    G_CALLBACK (parent_set_emission_hook_remove),
		    NULL);

  g_signal_connect (parent, "focus-in-event", 
		    G_CALLBACK(window_focus_cb),
		    cocoa_menubar);

  cocoa_menu_item_add_submenu (menu_shell, cocoa_menubar, TRUE, FALSE);
  create_window_menu (self, nswin);

}

GtkApplicationMenuGroup *
gtk_application_add_app_menu_group (GtkApplication* self )
{
  GNSMenuBar *menubar = (GNSMenuBar*)[NSApp mainMenu];
  GtkApplicationMenuGroup *group = [menubar addGroup];
    return group;
}

void
gtk_application_add_app_menu_item (GtkApplication *self,
				   GtkApplicationMenuGroup *group,
				   GtkMenuItem *menu_item)
{
  // we know that the application menu is always the submenu of the first item in the main menu
  GNSMenuBar *menubar = (GNSMenuBar*)[NSApp mainMenu];
  GList   *list = NULL, *menu_groups = [menubar app_menu_groups];
  gint     index = 0;
  NSMenu *app_menu = [[menubar itemAtIndex: 0] submenu];
  GtkWidget *parent = gtk_widget_get_toplevel (GTK_WIDGET (menu_item));

  g_return_if_fail (group != NULL);
  g_return_if_fail (GTK_IS_MENU_ITEM (menu_item));
  g_return_if_fail(parent != NULL);

  for (list = menu_groups; list; list = g_list_next (list))
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
	      [app_menu insertItem:[NSMenuItem separatorItem] 
	       atIndex:index+1];
	      index++;
	    }
	  DEBUG ("Add to APP menu bar %s\n", get_menu_label_text (GTK_WIDGET(menu_item), NULL));
	  cocoa_menu_item_add_item ([[[NSApp mainMenu] itemAtIndex: 0] submenu],
				    GTK_WIDGET(menu_item), index + 1);

	  group->items = g_list_append (group->items, menu_item);
	  gtk_widget_hide(GTK_WIDGET (menu_item));
	  return;
	}
    }

  if (!list)
    g_warning ("%s: app menu group %p does not exist",
	       G_STRFUNC, group);
}

