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

#ifndef __GTK_OSX_APPLICATION_H__
#define __GTK_OSX_APPLICATION_H__

#include <gtk/gtk.h>
#include <glib-object.h>


G_BEGIN_DECLS
#define GTKOSX_TYPE_APPLICATION (gtkosx_application_get_type())
G_DECLARE_FINAL_TYPE (GtkosxApplication, gtkosx_application, GTKOSX, APPLICATION, GObject)

typedef struct _GtkosxApplication GtkosxApplication;
typedef struct _GtkosxApplicationPrivate GtkosxApplicationPrivate;

/**
 * GtkosxApplicationMenuGroup:
 * @items: List of menu items in the group.
 *
 * A menu group is used to collect menu items between separators in
 * the Application menu.
 */
typedef struct _GtkosxApplicationMenuGroup GtkosxApplicationMenuGroup;

struct _GtkosxApplication
{
  GObject parent_instance;
  /*< private >*/
  GtkosxApplicationPrivate *priv;
};

struct _GtkosxApplicationClass
{
  GObjectClass parent_class;
};

struct _GtkosxApplicationMenuGroup
{
  GList *items;
};


GtkosxApplication *gtkosx_application_get (void);

//void gtkosx_application_init (GtkosxApplication *self);
void gtkosx_application_ready (GtkosxApplication *self);

/*Accelerator functions*/

void gtkosx_application_set_use_quartz_accelerators(GtkosxApplication *self, 
					 gboolean use_quartz_accelerators);
gboolean gtkosx_application_use_quartz_accelerators(GtkosxApplication *self);

/*Menu functions*/
void gtkosx_application_set_menu_bar (GtkosxApplication *self, 
				      GtkMenuShell *menu_shell);
void gtkosx_application_sync_menubar (GtkosxApplication *self);

void gtkosx_application_insert_app_menu_item (GtkosxApplication *self,
					      GtkWidget *menu_item,
					      gint index);
void gtkosx_application_set_about_item (GtkosxApplication* self,
                                        GtkWidget* item);
void gtkosx_application_set_window_menu (GtkosxApplication *self,
					 GtkMenuItem *menu_item);
void gtkosx_application_set_help_menu (GtkosxApplication *self,
				       GtkMenuItem *menu_item);

/*Dock Functions*/
/**
 * GtkosxApplicationAttentionType:
 * @GTKOSX_APPLICATION_ATTENTION_TYPE_CRITICAL_REQUEST: Bounce the icon until the app is activated.
 * @GTKOSX_APPLICATION_ATTENTION_TYPE_INFO_REQUEST: Bounce the icon for one second.
 * @CRITICAL_REQUEST (Deprecated): Bounce the icon until the app is activated.
 * @INFO_REQUEST (Deprecated): Bounce the icon for one second.
 *
 * The possible values for dock attention requests.
 */
typedef enum {
#ifndef GTKOSX_DISABLE_DEPRECATED
  CRITICAL_REQUEST = 0,
  INFO_REQUEST = 10,
#endif
  GTKOSX_APPLICATION_ATTENTION_TYPE_CRITICAL_REQUEST = 0,
  GTKOSX_APPLICATION_ATTENTION_TYPE_INFO_REQUEST = 10
} GtkosxApplicationAttentionType;

void gtkosx_application_set_dock_menu(GtkosxApplication *self, 
				   GtkMenuShell *menu_shell);
void gtkosx_application_set_dock_icon_pixbuf(GtkosxApplication *self,
					  GdkPixbuf *pixbuf);
void gtkosx_application_set_dock_icon_resource(GtkosxApplication *self,
					    const gchar  *name,
					    const gchar  *type,
					    const gchar  *subdir);
/* Gtk-mac-dock provided two functions,
 * gtk_mac_dock_set_overlay_from_pixbuf and
 * gtk_mac_doc_set_overlay_from_resource, but OSX 10.5 and later do
 * not support application dock tile overlays. Document dock tiles
 * will by default represent a miniaturized view of the document's
 * contents badged with an even more miniaturized application
 * icon. The interface to change this is a bit complex and will be
 * left up to the application rather than implemented here.
 */
gint gtkosx_application_attention_request(GtkosxApplication *self,
				       GtkosxApplicationAttentionType type);
void gtkosx_application_cancel_attention_request(GtkosxApplication *self, gint id);

/* Bundle Functions */
/* gtk-mac-bundle included a bunch of silly stuff for setting up the
 * environment. It's silly first because that's easier to do with a
 * startup script, and even easier to do with an LCEnvironment
 * dictionary in the bundle's Info.plist. 

 * Gtk applications, at least when launched with a shell script, still
 * return a bundle identifier and it's executable path is correct and
 * useful.  Gtk-mac-bundle had a "is it an application bundle"
 * function, but NSBundle doesn't provide that; instead,
 * quartz_application_get_bundle_id will return NULL if it's not really a
 * bundle, there's no Info.plist, or if Info.plist doesn't have a
 * CFBundleIdentifier key (So if you need to detect being in a bundle,
 * make sure that your bundle has that key!) 

 * Richard Proctor pointed out that these functions don't really need
 * to be class functions: the self parameter isn't used, and making
 * them "free" functions will often save one from having to call
 * g_object_new(GTKOSX_TYPE_APPLICATION) just to get it. */

gchar *gtkosx_application_get_bundle_path(void);
gchar *gtkosx_application_get_resource_path(void);
gchar *gtkosx_application_get_executable_path(void);
gchar *gtkosx_application_get_bundle_id(void);
gchar *gtkosx_application_get_bundle_info(const gchar *key);

G_END_DECLS

#endif /* __GTK_OSX_APPLICATION_H__ */
