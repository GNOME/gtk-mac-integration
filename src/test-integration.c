/* GTK+ Integration for the Mac OS X Menubar.
 *
 * Copyright (C) 2007 Imendio AB
 *
 * For further information, see:
 * http://developer.imendio.com/projects/gtk-macosx/menubar
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

#include "ige-mac-menu.h"
#include "ige-mac-dock.h"

static GtkWidget *open_item;
static GtkWidget *copy_item;
static GtkWidget *quit_item;
static GtkWidget *about_item;
static GtkWidget *preferences_item;

static void
dock_clicked_cb (IgeMacDock *dock,
                 GtkWindow  *window)
{
  g_print ("Dock clicked\n");

  gtk_window_deiconify (window);
}

static void
menu_item_activate_cb (GtkWidget *item,
                       gpointer   user_data)
{
  gboolean visible;
  gboolean sensitive;

  g_print ("Item activated: %s\n", (gchar *) user_data);

  g_object_get (G_OBJECT (copy_item),
                "visible", &visible,
                "sensitive", &sensitive,
                NULL);

  if (item == open_item) {
    gtk_widget_set_sensitive (copy_item, !sensitive);
    /*g_object_set (G_OBJECT (copy_item), "visible", !visible, NULL);*/
  }
}

static GtkWidget *
test_setup_menu (void)
{
  GtkWidget *menubar;
  GtkWidget *menu;
  GtkWidget *item;
  
  menubar = gtk_menu_bar_new ();

  item = gtk_menu_item_new_with_label ("File");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  item = gtk_menu_item_new_with_label ("Open");
  open_item = item;
  g_signal_connect (item, "activate", G_CALLBACK (menu_item_activate_cb), "open");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  quit_item = gtk_menu_item_new_with_label ("Quit");
  g_signal_connect (quit_item, "activate", G_CALLBACK (gtk_main_quit), NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), quit_item);

  item = gtk_menu_item_new_with_label ("Edit");

  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  item = gtk_menu_item_new_with_label ("Copy");
  copy_item = item;
  g_signal_connect (item, "activate", G_CALLBACK (menu_item_activate_cb), "copy");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

  item = gtk_menu_item_new_with_label ("Paste");
  g_signal_connect (item, "activate", G_CALLBACK (menu_item_activate_cb), "paste");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

  preferences_item = gtk_menu_item_new_with_label ("Preferences");
  g_signal_connect (preferences_item, "activate", G_CALLBACK (menu_item_activate_cb), "preferences");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), preferences_item);

  item = gtk_menu_item_new_with_label ("Help");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);

  about_item = gtk_menu_item_new_with_label ("About");
  g_signal_connect (about_item, "activate", G_CALLBACK (menu_item_activate_cb), "about");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), about_item);

  return menubar;
}

int
main (int argc, char **argv)
{
  GtkWidget       *window;
  GtkWidget       *vbox;
  GtkWidget       *menubar;
  IgeMacMenuGroup *group;
  IgeMacDock      *dock;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  menubar = test_setup_menu ();
  gtk_box_pack_start (GTK_BOX (vbox), 
                      menubar,
                      FALSE, TRUE, 0);
  
  gtk_box_pack_start (GTK_BOX (vbox), 
                      gtk_label_new ("Some window content here"), 
                      TRUE, TRUE, 0);

  gtk_widget_show_all (window);

  gtk_widget_hide (menubar);

  ige_mac_menu_set_menu_bar (GTK_MENU_SHELL (menubar));
  ige_mac_menu_set_quit_menu_item (GTK_MENU_ITEM (quit_item));

  group = ige_mac_menu_add_app_menu_group ();
  ige_mac_menu_add_app_menu_item  (group,
                                   GTK_MENU_ITEM (about_item), 
                                   NULL);

  group = ige_mac_menu_add_app_menu_group ();
  ige_mac_menu_add_app_menu_item  (group,
                                   GTK_MENU_ITEM (preferences_item), 
                                   NULL);
  
  dock = ige_mac_dock_new ();
  g_signal_connect (dock,
                    "clicked",
                    G_CALLBACK (dock_clicked_cb),
                    window);
  g_signal_connect (dock,
                    "quit-activate",
                    G_CALLBACK (gtk_main_quit),
                    window);

  gtk_main ();

  return 0;
}
