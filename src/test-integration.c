/* GTK+ Integration for the Mac OS X.
 *
 * Copyright (C) 2007 Imendio AB
 * Copyright © 2009, 2010 John Ralls
 *
 * For further information, see:
 * http://sourceforge.net/apps/trac/gtk-osx/wiki/Integrate
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
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

/* - Dock menu (see
 *   https://bugzilla.mozilla.org/show_bug.cgi?id=377166) It looks
 *   like we have to use NSMenu for this unfortunately. However, it
 *   might be possible to implement a category or to use the "method
 *   swizzling" trick to override this somehow... look into that.
 * 
 *   or, triggering the carbon dock setup before sharedapp:
 *
 *     EventTypeSpec kFakeEventList[] = { { INT_MAX, INT_MAX } };
       EventRef event;
       ReceiveNextEvent (GetEventTypeCount (kFakeEventList),
                         kFakeEventList,
                         kEventDurationNoWait, false, 
                         &event);
 *
 * - Dragging items onto the dock icon?
 * - Dragging items onto the app launcher icon?
 *
 * - Handling many windows with menus...?
 *
 * - Better statusicon integration (real menu)
 *
 * - "Window" menu, add a way to add a standard Window menu.
 * - Window listing in the dock menu.
 *
 * - Implement moving borderless windows by dragging the window
 *   background.
 *
 * - Suspend/resume notification.
 * - Network on/off notification.
 * - GtkCheckMenuItems
 * - GtkRadioMenuItems (special case of GtkCheckMenuItems)
 */

/* Uncomment ONE of these to test menu-mangling: */
//#define IGEMACMENU
#define GTKAPPLICATION
#define BUILT_UI
#define QUARTZ_HANDLERS

#include <gtk/gtk.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>

#ifdef IGEMACMENU
#include "ige-mac-menu.h"
#include "ige-mac-dock.h"
#include "ige-mac-bundle.h"
#endif
#ifdef GTKAPPLICATION
#include "gtkapplication.h"
#endif
#include <config.h>

typedef struct {
  GtkWindow *window;
  GtkWidget *open_item;
  GtkWidget *edit_item;
  GtkWidget *copy_item;
  GtkWidget *quit_item;
  GtkWidget *about_item;
  GtkWidget *preferences_item;
} MenuItems;

static GQuark menu_items_quark = 0;

static MenuItems *
menu_items_new() {
    return g_slice_new0 (MenuItems);
}

static void
menu_items_destroy(MenuItems *items) {
    g_slice_free(MenuItems, items);
}

typedef struct {
    gchar *label;
    gpointer item;
} MenuCBData;

static MenuCBData *
menu_cbdata_new (gchar *label, gpointer item) {
    MenuCBData *datum =  g_slice_new0 (MenuCBData);
    datum->label = label;
    datum->item = item;
    g_object_ref (datum->item);
    return datum;
}

static void
menu_cbdata_delete (MenuCBData *datum) {
    g_object_unref (datum->item);
    g_slice_free (MenuCBData, datum);
}

#ifdef BUILT_UI

static void
action_activate_cb(GtkAction* action, gpointer data)
{
  GtkWindow *window = data;
  g_print("Window %s, Action %s\n", gtk_window_get_title(window),
	  gtk_action_get_name(action));
}

static GtkActionEntry test_actions[] = 
  {
    /*{Name, stock_id, label, accelerator, tooltip, callback} */
    {"FileMenuAction", NULL, "_File", NULL, NULL, NULL},
    {"OpenAction",  GTK_STOCK_OPEN, "_Open", NULL, NULL, 
     G_CALLBACK(action_activate_cb)},
    {"QuitAction", GTK_STOCK_QUIT, "_Quit", NULL, NULL,
     G_CALLBACK(gtk_main_quit)},
    {"EditMenuAction", NULL, "_Edit", NULL, NULL, NULL },
    {"CopyAction", GTK_STOCK_COPY, "_Copy", NULL, NULL,
     G_CALLBACK(action_activate_cb)},
    {"PasteAction", GTK_STOCK_PASTE, "_Paste", NULL, NULL,
     G_CALLBACK(action_activate_cb)},
    {"PrefsAction", GTK_STOCK_PREFERENCES, "Pr_eferences", NULL, NULL, NULL },
    {"HelpMenuAction", NULL, "_Help", NULL, NULL, NULL },
    {"AboutAction", GTK_STOCK_ABOUT, "_About", NULL, NULL,
     G_CALLBACK(action_activate_cb)},
    {"HelpAction", GTK_STOCK_HELP, "_Help", NULL, NULL, 
     G_CALLBACK(action_activate_cb)},
  };

static void
radio_item_changed_cb (GtkAction* action, GtkAction* current, MenuCBData *datum)
{
    g_print ("Radio group %s in window %s changed value: %s is now active.\n", 
	     datum->label, gtk_window_get_title(GTK_WINDOW(datum->item)), 
	     gtk_action_get_name(GTK_ACTION(current)));
}

static GtkActionEntry view_menu[] =
{
    {"ViewMenuAction", NULL, "_View", NULL, NULL, NULL},
};

static GtkRadioActionEntry view_actions[] = 
{
/* Name, StockID, Label, Accelerator, Tooltip, Value */
    {"HorizontalAction", NULL, "_Horizontal", NULL, NULL, 0},
    {"VerticalAction", NULL, "_Vertical", NULL, NULL, 0},
};
#else //not BUILT_UI

/* This is needed as a callback to enable accelerators when not using
 * the Quartz event handling path and using GtkMenuItems instead of
 * GtkActions, otherwise hiding the menu disables accelerators. */

static gboolean
can_activate_cb(GtkWidget* widget, guint signal_id, gpointer data)
{
  return gtk_widget_is_sensitive(widget);
}


static GtkWidget *
test_setup_menu (MenuItems *items, GtkAccelGroup *accel)
{
  GtkWidget *menubar;
  GtkWidget *menu;
  GtkWidget *item;
  menubar = gtk_menu_bar_new ();

  item = gtk_menu_item_new_with_label ("File");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_set_accel_group(GTK_MENU(menu), accel);
  gtk_menu_set_accel_path(GTK_MENU(menu), "<test-integration>/File");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  item = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  items->open_item = item;
/* We're being fancy with our connection here so that we don't have to
 * have a separate callback function for each menu item, since each
 * one is going to print out a message saying what item got
 * selected. A real-life menu item usually uses just
 * g-signal_connect() to a dedicated callback -- or uses GuiManager
 * action closures.
 */
  g_signal_connect_data (item, "activate", G_CALLBACK (menu_item_activate_cb),
			 menu_cbdata_new ("open", items->window),
			 (GClosureNotify) menu_cbdata_delete, 0);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  items->quit_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
  g_signal_connect (items->quit_item, "activate", G_CALLBACK (gtk_main_quit), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), items->quit_item);
//Set accelerators
  gtk_accel_map_add_entry("<test-integration>/File/Open", GDK_o, 
			  GDK_CONTROL_MASK);
  gtk_accel_map_add_entry("<test-integration>/File/Quit", GDK_q, 
			  GDK_CONTROL_MASK);
  items->edit_item = item = gtk_menu_item_new_with_label ("Edit");

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  item = gtk_menu_item_new_with_label ("Copy");
  items->copy_item = item;
  g_signal_connect_data (item, "activate", G_CALLBACK (menu_item_activate_cb),
			 menu_cbdata_new ("copy", items->window),
			 (GClosureNotify) menu_cbdata_delete, 0);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

  item = gtk_menu_item_new_with_label ("Paste");
  g_signal_connect_data (item, "activate", G_CALLBACK (menu_item_activate_cb),
			 menu_cbdata_new ( "paste", items->window),
			 (GClosureNotify) menu_cbdata_delete, 0);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

  items->preferences_item = gtk_menu_item_new_with_label ("Preferences");
  g_signal_connect_data (items->preferences_item, "activate", 
			 G_CALLBACK (menu_item_activate_cb), 
			 menu_cbdata_new ("preferences", items->window),
			 (GClosureNotify) menu_cbdata_delete, 0);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), items->preferences_item);

  item = gtk_menu_item_new_with_label ("Help");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);

  items->about_item = gtk_menu_item_new_with_label ("About");
  g_signal_connect_data (items->about_item, "activate", 
			 G_CALLBACK (menu_item_activate_cb), 
			 menu_cbdata_new ("about", items->window),
			 (GClosureNotify) menu_cbdata_delete, 0);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), items->about_item);

  return menubar;
}
#endif //not BUILT_UI

static void
menu_item_activate_cb (GtkWidget *item,
                       MenuCBData  *datum)
{
  gboolean visible;
  gboolean sensitive;
  MenuItems *items = g_object_get_qdata (G_OBJECT(datum->item),
					 menu_items_quark);
  if (GTK_IS_WINDOW(G_OBJECT(datum->item)))
      g_print ("Item activated: %s:%s\n",
	       gtk_window_get_title(GTK_WINDOW(datum->item)),
	       datum->label);
  else
    g_print ("Item activated %s\n", datum->label);

  if (!items)
    return;

  g_object_get (G_OBJECT (items->copy_item),
                "visible", &visible,
                "sensitive", &sensitive,
                NULL);

  if (item == items->open_item) {
    gtk_widget_set_sensitive (items->copy_item,
			      !gtk_widget_get_sensitive(items->copy_item));
  }
}

#ifdef IGEMACMENU
static void
dock_clicked_cb (IgeMacDock *dock,
                 GtkWindow  *window)
{
  g_print ("Dock clicked\n");
  gtk_window_deiconify (window);
}

static void
bounce_cb (GtkWidget  *button,
           IgeMacDock *dock)
{
  ige_mac_dock_attention_request (dock, IGE_MAC_ATTENTION_INFO);
}

static void
change_icon_cb (GtkWidget  *button,
                IgeMacDock *dock)
{
  static gboolean   changed;
  static GdkPixbuf *pixbuf;

  if (!pixbuf) {
      char filename[PATH_MAX];
      snprintf (filename, sizeof(filename), "%s/%s", PREFIX, 
		 "share/gtk-2.0/demo/gnome-foot.png");
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
  }

  if (changed) 
    ige_mac_dock_set_icon_from_pixbuf (dock, NULL);
  else
    ige_mac_dock_set_icon_from_pixbuf (dock, pixbuf);

  changed = !changed;
}
#elif defined GTKAPPLICATION
typedef struct {
  GtkApplication *app;
  GtkApplicationAttentionType type;
} AttentionRequest;

static gboolean
attention_cb(AttentionRequest* req)
{
  gtk_application_attention_request(req->app, req->type);
  g_free(req);
  return FALSE;
}

static void
bounce_cb (GtkWidget  *button,
           GtkApplication *app)
{
  AttentionRequest *req = g_new0 (AttentionRequest, 1);
  req->app = app;
  req->type = CRITICAL_REQUEST;
  g_timeout_add_seconds(2, (GSourceFunc)attention_cb, req);
  g_print("Now switch to some other application\n");
}

static void
change_icon_cb (GtkWidget  *button,
                GtkApplication *app)
{
  static gboolean   changed;
  static GdkPixbuf *pixbuf;
  if (!pixbuf) {
      char filename[PATH_MAX];
      snprintf (filename, sizeof(filename), "%s/%s", PREFIX,
		 "share/gtk-2.0/demo/gnome-foot.png");
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
  }

  if (changed)
    gtk_application_set_dock_icon_pixbuf (app, NULL);
  else
    gtk_application_set_dock_icon_pixbuf (app, pixbuf);

  changed = !changed;
}
#endif

static void
change_menu_cb (GtkWidget  *button,
                gpointer    user_data)
{
  GtkWidget *window = gtk_widget_get_toplevel(button);
  MenuItems *items = g_object_get_qdata(G_OBJECT(window), menu_items_quark);
  const gchar* open_accel_path =
      gtk_menu_item_get_accel_path(GTK_MENU_ITEM(items->open_item));
  const gchar* quit_accel_path =
      gtk_menu_item_get_accel_path(GTK_MENU_ITEM(items->quit_item));

  if (gtk_widget_get_visible(items->edit_item)) {
      gtk_widget_set_visible(items->edit_item, FALSE);
      gtk_accel_map_change_entry(open_accel_path, GDK_o,
				 GDK_META_MASK, TRUE);
      gtk_accel_map_change_entry(quit_accel_path, GDK_q,
				 GDK_META_MASK, TRUE);
  } else {
     gtk_widget_set_visible(items->edit_item, TRUE);
     gtk_accel_map_change_entry(open_accel_path, GDK_o,
				GDK_CONTROL_MASK, TRUE);
     gtk_accel_map_change_entry(quit_accel_path, GDK_q,
				GDK_CONTROL_MASK, TRUE);
  }
}

static void
view_menu_cb (GtkWidget *button, gpointer user_data)
{
  GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(button);
#ifdef BUILT_UI
  static guint mergeid = 0;
  static GtkActionGroup* view_action_group = NULL;
  GtkUIManager *mgr = user_data;
  GtkWidget *window = gtk_widget_get_toplevel(button);
  GtkApplication *theApp = g_object_new(GTK_TYPE_APPLICATION, NULL);
  GError *err = NULL;
  if (view_action_group == NULL) {
    view_action_group = gtk_action_group_new("ViewAction");
    gtk_action_group_add_actions(view_action_group, view_menu,
				 sizeof(view_menu)/sizeof(GtkActionEntry),
				 NULL);
    gtk_action_group_add_radio_actions_full(
	    view_action_group, view_actions, 
	    sizeof(view_actions)/sizeof(GtkRadioActionEntry), 
	    0, G_CALLBACK(radio_item_changed_cb),
	    menu_cbdata_new ("View", GTK_WINDOW(window)),
	    (GDestroyNotify) menu_cbdata_delete );
  }
  if (gtk_toggle_button_get_active(toggle)) {
    mergeid = gtk_ui_manager_add_ui_from_file(mgr, "src/addedui.xml", &err);
    if (err) {
      g_print("Error retrieving file: %d %s\n", mergeid, err->message);
    }
    gtk_ui_manager_insert_action_group(mgr, view_action_group, 0);
    {
      GtkWidget *menu = gtk_menu_new();
      GtkWidget *item;
      item = gtk_menu_item_new_with_label("Framish");
      g_signal_connect_data (item, "activate",
			     G_CALLBACK (menu_item_activate_cb),
			     menu_cbdata_new ( "Framish", item),
			     (GClosureNotify) menu_cbdata_delete, 0);
      gtk_menu_append(menu, item);
      item = gtk_menu_item_new_with_label("Freebish");
      g_signal_connect_data (item, "activate",
			     G_CALLBACK (menu_item_activate_cb),
			     menu_cbdata_new ( "Freebish", item),
			     (GClosureNotify) menu_cbdata_delete, 0);
      gtk_menu_append(menu, item);
      gtk_application_set_dock_menu(theApp, GTK_MENU_SHELL(menu));
    }
  }
  else if (mergeid) {
    gtk_ui_manager_remove_action_group(mgr, view_action_group);
    gtk_ui_manager_remove_ui(mgr, mergeid);
    mergeid = 0;
  }
#else
  g_print("View Menu Toggle Button doesn't actually do anything in the hand-built menu build\n");
#endif //BUILT_UI
}

gboolean _ige_mac_menu_is_quit_menu_item_handled (void);

#ifdef IGEMACMENU
static GtkWidget *
create_window(IgeMacDock *dock, const gchar *title)
{
#else
static GtkWidget *
create_window(const gchar *title)
{
  gpointer	dock = NULL;
#endif
  GtkWidget	  *window;
  GtkWidget       *vbox;
  GtkWidget       *menubar;
  GtkWidget       *bbox;
  GtkWidget       *button;
  MenuItems       *items = menu_items_new();
#ifdef BUILT_UI
  GtkUIManager *mgr = gtk_ui_manager_new();
  GtkActionGroup *actions = gtk_action_group_new("TestActions");
  GtkAccelGroup *accel_group;
  guint mergeid;
  GError *err = NULL;
#else //not BUILT_UI
  GtkAccelGroup *accel_group = gtk_accel_group_new();
#endif //not BUILT_UI
#ifdef IGEMACMENU
  IgeMacMenuGroup *group;
#endif //IGEMACMENU
#ifdef GTKAPPLICATION
  GtkApplicationMenuGroup *group;
  GtkApplication *theApp = g_object_new(GTK_TYPE_APPLICATION, NULL);
#endif //GTKAPPLICATION
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  if (title)
      gtk_window_set_title (GTK_WINDOW (window), title);
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  items->window = GTK_WINDOW (window);
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
#ifdef BUILT_UI
  mergeid = gtk_ui_manager_add_ui_from_file(mgr, "src/testui.xml", &err);
  if (err) {
    g_print("Error retrieving file: %d %s\n", mergeid, err->message);
  }
  gtk_action_group_add_actions(actions, test_actions,
			       sizeof(test_actions)/sizeof(GtkActionEntry),
			       (gpointer)window);
  gtk_ui_manager_insert_action_group(mgr, actions, 0);
  menubar = gtk_ui_manager_get_widget(mgr, "/menubar");
  items->open_item = gtk_ui_manager_get_widget(mgr, "/menubar/File/Open");
  items->edit_item = gtk_ui_manager_get_widget(mgr, "/menubar/Edit");
  items->copy_item = gtk_ui_manager_get_widget(mgr, "/menubar/Edit/Copy");
  items->quit_item = gtk_ui_manager_get_widget(mgr, "/menubar/File/Quit");
  items->about_item = gtk_ui_manager_get_widget(mgr, "/menubar/Help/About");
  items->preferences_item = gtk_ui_manager_get_widget(mgr, "/menubar/Edit/Preferences");
  accel_group = gtk_ui_manager_get_accel_group(mgr);
#else //not BUILT_UI
  menubar = test_setup_menu (items, accel_group);
#endif //not BUILT_UI
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
  gtk_box_pack_start (GTK_BOX (vbox), 
                      menubar,
                      FALSE, TRUE, 0);
  
  gtk_box_pack_start (GTK_BOX (vbox), 
                      gtk_label_new ("Some window content here"), 
                      FALSE, FALSE, 12);

  bbox = gtk_hbutton_box_new ();
  gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_CENTER);
  gtk_button_box_set_spacing (GTK_BUTTON_BOX (bbox), 12);

  gtk_box_pack_start (GTK_BOX (vbox), 
                      bbox,
                      TRUE, TRUE, 0);

  button = gtk_button_new_with_label ("Bounce");
  g_signal_connect (button, "clicked", G_CALLBACK (bounce_cb), dock);
  gtk_box_pack_start (GTK_BOX (bbox), 
                      button,
                      FALSE, FALSE, 0);

  button = gtk_button_new_with_label ("Change Icon");
  g_signal_connect (button, "clicked", G_CALLBACK (change_icon_cb), dock);
  gtk_box_pack_start (GTK_BOX (bbox), 
                      button,
                      FALSE, FALSE, 0);

  button = gtk_button_new_with_label ("Change Menu");
  g_signal_connect (button, "clicked", G_CALLBACK (change_menu_cb), NULL);
  gtk_box_pack_start (GTK_BOX (bbox),
                      button,
                      FALSE, FALSE, 0);
  button = gtk_toggle_button_new_with_label("View Menu");
#ifdef BUILT_UI
  g_signal_connect(button, "toggled", G_CALLBACK (view_menu_cb), (gpointer)mgr);
#else //not BUILT_UI
  g_signal_connect(button, "toggled", G_CALLBACK (view_menu_cb), NULL);
#endif //not BUILT_UI
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
  gtk_box_pack_start (GTK_BOX (bbox),
		      button,
		      FALSE, FALSE, 0);

  gtk_widget_show_all (window);
#if defined IGE_MAC_MENU || defined GTKAPPLICATION
  gtk_widget_hide (menubar);
#ifdef GTKAPPLICATION
/* Not really necessary unless quartz accelerator handling is turned off. */
#if !defined QUARTZ_HANDLERS && !defined BUILT_UI
  g_signal_connect(menubar, "can-activate-accel", 
		   G_CALLBACK(can_activate_cb), NULL);
#endif // !defined QUARTZ_HANDLERS && !defined BUILT_UI
#endif  //GTKAPPLICATION
#endif //defined IGE_MAC_MENU || defined GTKAPPLICATION
#ifdef IGEMACMENU
  ige_mac_menu_set_menu_bar (GTK_MENU_SHELL (menubar));
  ige_mac_menu_set_quit_menu_item (GTK_MENU_ITEM (items->quit_item));
  group = ige_mac_menu_add_app_menu_group ();
  ige_mac_menu_add_app_menu_item  (group,
				   GTK_MENU_ITEM (items->about_item),
				   "About");
  group = ige_mac_menu_add_app_menu_group ();
  ige_mac_menu_add_app_menu_item  (group,
				   GTK_MENU_ITEM (items->preferences_item),
				   "Preferences");
#endif //IGEMACMENU
#ifdef GTKAPPLICATION
  gtk_application_set_menu_bar(theApp, GTK_MENU_SHELL(menubar));
  group = gtk_application_add_app_menu_group (theApp);
  gtk_application_add_app_menu_item  (theApp, group,
				      GTK_MENU_ITEM (items->about_item));

  group = gtk_application_add_app_menu_group (theApp);
  gtk_application_add_app_menu_item  (theApp, group,
				      GTK_MENU_ITEM (items->preferences_item));
#endif //GTKAPPLICATION
  if (!menu_items_quark)
      menu_items_quark = g_quark_from_static_string("MenuItem");
  g_object_set_qdata_full(G_OBJECT(window), menu_items_quark, 
			  items, (GDestroyNotify)menu_items_destroy);
  return window;
}

int
main (int argc, char **argv)
{
  GtkWidget       *window1, *window2;
#ifdef IGEMACMENU
  IgeMacDock      *dock;
#endif //IGEMACMENU
#ifdef GTKAPPLICATION
  GtkApplication *theApp;
#endif //GTKAPPLICATION
  gtk_init (&argc, &argv);
#ifdef IGEMACMENU
  dock = ige_mac_dock_get_default ();
  window1 = create_window(dock, "Test Integration Window 1"); 
  window2 = create_window(dock, "Test Integration Window 2"); 
  dock = ige_mac_dock_new ();
  g_signal_connect (dock,
                    "clicked",
                    G_CALLBACK (dock_clicked_cb),
                    window1);
  g_signal_connect (dock,

                    "quit-activate",
                    G_CALLBACK (gtk_main_quit),
                    window1);
#endif //IGEMACMENU
#ifdef GTKAPPLICATION
  theApp  = g_object_new(GTK_TYPE_APPLICATION, NULL);
  window1 = create_window("Test Integration Window 1");
  window2 = create_window("Test Integration Window 2");
#ifndef QUARTZ_HANDLERS
  gtk_application_set_use_quartz_accelerators(theApp, FALSE);
#endif //QUARTZ_HANDLERS
  gtk_application_ready(theApp);
#endif //GTKAPPLICATION
  gtk_main ();

#ifdef GTKAPPLICATION
  g_object_unref(theApp);
#endif //GTKAPPLICATION
  return 0;
}
