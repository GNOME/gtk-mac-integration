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
 * SECTION:gtkosxapplication
 * @short_description: Base class for OS X integration
 * @title: GtkosxApplication
 * @include: gtkosxapplication.h
 *
 * Exposes to the Gtk+ program important functions of
 * OS X's NSApplication class for use by Gtk+ applications running with
 * the quartz Gdk backend and provides addtional functions for
 * integrating a Gtk+ program into the macOS user environment.
 *
 * Note: There's a complete example program in src/testintegration.c.
 *
 * Note: This library is obsolescent in Gtk3 as there are built-in
 * features in GLib and Gtk to integrate Gtk applications with macOS.
 *
 * Note: gtkosxapplication does not and cannot support Gtk4. Use the
 * built-in macOS integration features.
 *
 * Using GtkosxApplication is pretty simple.
 * First, create an instance at startup:
 *
 * |[<!-- language="C" -->
 * GtkosxApplication *theApp = g_object_new(GTKOSX_TYPE_APPLICATION, NULL);
 * ]|
 *
 * Do this early in your program, shortly after you run
 * |[
 * gtk_init()
 * ]|
 * Don't forget to guard it, and all other calls into the library, with
 * |[
 * #ifdef MAC_INTEGRATION
 * ]|
 *
 * You don't want your Linux users' builds failing because of this.
 * The application object is a singleton, so you can call g_object_new
 * as often as you like. You'll always get the same pointer
 * back. There's no need to pass it around as an argument. Do note
 * that all of the GtkosxApplication functions take theApp as an
 * argument, even if they don't use it. This seems silly in C, and
 * perhaps it is, but it's needed to make the Python binding logic
 * recognize that they're class methods.
 *
 * Just having the application object created will get you some
 * benefits, like having the Quit menu item in the dock menu work. But
 * you'll obviously want more. So the next place to visit is your main
 * window code. If you have a simple application, you might be
 * constructing the menu by hand, but you're more likely to be using
 * GtkBuilder. In either case, you need to get a pointer to the
 * menubar. If you're building by hand, you've already got it lying
 * around because you needed it to add the menus to. With GtkBuilder,
 * you need to ask the GtkUIManager for a pointer. Once everything is
 * more-or-less set up on the Gtk+ side, you need only hide the menu
 * and call gtkosx_application_set_main_menu(). Here's an example with
 * GtkBuilder:
 *
 * ## Setting the MenuBar
 * |[<!-- language="C" -->
 *   GtkUIManager *mgr = gtk_ui_manager_new();
 *   GtkosxApplication *theApp = g_object_new(GTKOSX_TYPE_APPLICATION, NULL);
 *   ...
 *   mergeid = gtk_ui_manager_add_ui_from_file(mgr, "src/testui.xml", &err);
 *   ...
 *   menubar = gtk_ui_manager_get_widget(mgr, "/menubar");
 *   gtk_widget_hide (menubar);
 *   gtkosx_application_set_menu_bar(theApp, GTK_MENU_SHELL(menubar));
 * ]|
 *
 * There are a couple of wrinkles, though, if you use
 * accelerators. First off, there are two event paths for
 * accelerators: Quartz, where the keystroke is processed by OS X and
 * the menu item action event is placed on the event queue by OS X, or
 * Gtk, where the accelerator key event is passed through to Gtk to
 * recognize. This is controlled by
 * gtkosx_application_set_use_quartz_accelerators() (you can test the
 * value with gtkosx_application_use_quartz_accelerators()), and the
 * default is to use Quartz handling. This has two advantages:
 * - It works without any extra steps
 * - It changes stock accelerators (like Ctrl-O for open file) to
 * the stock OS X keyEquivalent (Cmd-O in that case).
 *
 * If you need to use Gtk+ keyboard accelerator handling *and*
 * you're using GtkMenuItems instead of GtkActions, you'll need to
 * connect a special handler as shown in the following example:
 * ## Enabling Accelerators on Hidden Menus
 * |[<!-- language="C" -->
 * static gboolean
 * can_activate_cb(GtkWidget* widget, guint signal_id, gpointer data)
 * {
 *   return gtk_widget_is_sensitive(widget);
 * }
 * ...
 *   g_signal_connect(menubar, "can-activate-accel",
 *                    G_CALLBACK(can_activate_cb), NULL);
 * ]|
 *
 * The next task to make your application appear more normal for Mac
 * users is to move some menu items from their normal Gtk locations to
 * the so-called "App" menu. That's the menu all the way at the left
 * of the menubar that has the currently-running application's
 * name. There are 3 menu items that normally go there:
 * - Help|About
 * - Edit|Preferences
 * - File|Quit
 *
 * File|Quit is a special case, because OS X handles it itself and
 * automatically includes it, so the only thing you need do is hide it
 * on the File menu so that it doesn't show up twice:
 * |[
 * gtk_widget_hide(GTK_WIDGET(file_quit_menu_item));
 * ]|
 * The other two must be moved in code. That's done by inserting them
 * into the app menu. The following example demonstrates this along
 * with placing separators above and below to set off our new menu
 * items.
 * |[
 *  GtkosxApplicationMenuGroup *group;
 *  GtkMenuItem *about_item, *preferences_item;
 *  about_item = gtk_ui_manager_get_widget(mgr, "/menubar/Help/About");
 *  preferences_item = gtk_ui_manager_get_widget(mgr, "/menubar/Edit/Preferences");
 *  sep = gtk_separator_menu_item_new ();
 *  g_object_ref (sep);
 *  gtkosx_application_insert_app_menu_item  (theApp, sep, 1);

 *
 *  gtkosx_application_insert_app_menu_item  (theApp,
 *                                            GTK_MENU_ITEM (about_item), 1);
 *
 *  gtkosx_application_insert_app_menu_item  (theApp,
 *                                            GTK_MENU_ITEM (preferences_item), 2);
 *  sep = gtk_separator_menu_item_new ();
 *  g_object_ref (sep);
 *  gtkosx_application_insert_app_menu_item  (theApp, sep, 1);
 * ]|
 * Once we have everything set up for as many windows as we're going
 * to open before we call gtk_main_loop(), we need to tell OS X that
 * we're ready:
 * |[
 * gtkosx_application_ready(theApp);
 * ]|
 *
 * If you add other windows later, you must do everything above for
 * each one's menubar. Most of the time the internal notifictations
 * will ensure that the GtkosxApplication is able to keep everything
 * in sync. However, if you at any time disconnect or block signals
 * and change the menu (perhaps because of a context change within a
 * window, as with changing pages in a GtkNotebook) you need to call
 * |[
 * gtkosx_application_sync_menubar(theApp)
 * ]|
 *
 * N.B.: One GtkMenu function, gtk_menu_reorder_child(), changes the
 * menu appearance without emitting a signal, so if you use that
 * function in your code you'll need to call
 * gtkosx_application_sync_menubar() afterwards.
 *
 * ## Dock Support
 * The dock is that bar of icons that normally lives at the bottom of
 * the display on a Mac (though it can be moved to one of the other
 * sides; this author likes his on the left, which is where it was
 * originally on a NeXT). Each running application has a "dock tile",
 * an icon on the dock. Users can, if they like, add application (or
 * document) icons to the dock, and those can be used to launch the
 * application. Apple allows limited customization of the dock tile,
 * and GtkosxApplication has an interface for adding to the dock's
 * menu and for changing the icon that is displayed for the the
 * application. GtkosxApplication also provides an interface to
 * AttentionRequest, which bounces the dock tile if the application
 * doesn't have focus. You might want to do that at the end of a long
 * task so that the user will know that it's finished if she's
 * switched to another application while she waits for yours.
 * They're all pretty simple, so you can just read the details below.
 * - gtkosx_application_set_doc_menu()
 * - gtkosx_application_set_doc_icon_pixbuf()
 * - gtkosx_application_set_dock_icon_resource()
 * - gtkosx_application_attention_request()
 * - gtkosx_application_cancel_attention_request()
 *
 * ## Bundle Support
 * The last feature to which GtkosxApplication provides an interface
 * is the bundle. Normally in OS X, graphical applications are packaged
 * along with their non-standard dependencies and their resources
 * (graphical elements, translations, and such) in special directory
 * structures called "bundles". To easily package your Gtk+
 * application, have a look at gtk-mac-bundler, also available from
 * the Gtk-OSX project.
 *
 * OS X provides a variety of functions pertaining to bundles, most of
 * which are not likely to interest someone porting a Gtk+
 * application. GtkosxApplication has wrapped a few that might be:
 * - gtkosx_application_get_bundle_path()
 * - gtkosx_application_get_resource_path()
 * - gtkosx_application_get_executable_path()
 * - gtkosx_application_get_bundle_id()
 * - gtkosx_application_get_bundle_info()
 *
 * The first three just get a UTF8-encoded path. An interesting note
 * is that they'll return the path to the executable or the folder
 * it's in regardless of whether it's actually in a bundle. To find
 * out if one is actually dealing with a bundle,
 * gtkosx_application_get_bundle_id() will return "" if it can't find
 * the key %CFBundleIdentifier from the bundle's Info.plist -- which it
 * won't if the application isn't in a bundle or wasn't launched by
 * opening the bundle. (In other words, even if you have your
 * application installed in Foo.app, if you launch it from the command
 * line as
 * |[
 * $ Foo.app/Contents/MacOS/Foo
 * ]|
 * the Info.plist won't have been opened and
 * gtkosx_application_get_bundle_id() will return "". Of course, it
 * will also return "" if you didn't set %CFBundleIdentifier in the
 * Info.plist, so make sure that you do!
 *
 * The last function, gtkosx_application_get_bundle_info(), will
 * return the value associated with an arbitrary key from Info.plist
 * as long as that value is a string. If it isn't, then the function
 * returns a null string ("").
 *
 * ## Notifications
 * Finally, notice the signals. These are emitted in response to the
 * indicated OS X notifications. Except for
 * #GtkosxApplication::NSApplicationBlockTermination, most programs
 * won't need to do anything with
 * them. #GtkosxApplication::NSApplicationBlockTermination is telling
 * you that OS X is planning to shut down your program. If you have any
 * cleanup to do (like saving open files), or if you want to ask the
 * user if it's OK, you should connect to the signal and do your
 * cleanup. Your handler can return %TRUE to prevent the application
 * from quitting.
 */

/**
 * gtkosx_application_get:
 *
 * Get the singleton NSApplication object.
 *
 * Returns: (transfer none): the singleton application object.
 *
 */
GtkosxApplication *
gtkosx_application_get (void)
{
  return g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
}


/**
 * gtkosx_application_use_quartz_accelerators:
 * @self: The GtkosxApplication pointer.
 *
 * Are we using Quartz or Gtk+ accelerator handling?
 *
 * Returns: a gboolean
 */
gboolean
gtkosx_application_use_quartz_accelerators (GtkosxApplication *self)
{
  return self->priv->use_quartz_accelerators;
}

/**
 * gtkosx_application_set_use_quartz_accelerators:
 * @self: The GtkosxApplication pointer.
 * @use_quartz_accelerators: Gboolean
 *
 * Set quartz accelerator handling; TRUE (default) uses quartz; FALSE
 * uses Gtk+. Quartz accelerator handling is required for normal OS X
 * accelerators (e.g., command-q to quit) to work.
 */
void
gtkosx_application_set_use_quartz_accelerators (GtkosxApplication *self,
    gboolean use_quartz_accelerators)
{
  self->priv->use_quartz_accelerators = use_quartz_accelerators;
}
