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

#ifndef __GTK_APPLICATION_H__
#define __GTK_APPLICATION_H__

#include <gtk/gtk.h>
#include <glib-object.h>

G_BEGIN_DECLS
#define GTK_TYPE_APPLICATION	(gtk_application_get_type())
#define GTK_APPLICATION(obj) 	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_APPLICATION, GtkApplication))
#define GTK_IS_APPLICATION(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_APPLICATION))
#define GTK_APPLICATION_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass),  GTK_TYPE_APPLICATION, GtkApplicationClass))
#define GTK_IS_APPLICATION_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass),  GTK_TYPE_APPLICATION))
#define GTK_APPLICATION_GET_CLASS(obj) 	(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_APPLICATION, GtkApplicationClass))

typedef struct _GtkApplication GtkApplication;
typedef struct _GtkApplicationPrivate GtkApplicationPrivate;
typedef struct _GtkApplicationClass GtkApplicationClass;
typedef struct _GtkApplicationMenuGroup GtkApplicationMenuGroup;

struct _GtkApplication
{
  GObject parent_instance;
  /*< private >*/
  GtkApplicationPrivate *priv;
};

struct _GtkApplicationClass
{
  GObjectClass parent_class;
  void (*activation_changed) (GtkApplication *self, gboolean changed);
  void (*should_load) (GtkApplication *self, gchar *utf8_path);
  void (*should_quit) (GtkApplication *self);
};

struct _GtkApplicationMenuGroup
{
  GList *items;
};


GType gtk_application_get_type (void);
//GtkApplication *gtk_application_get (void);

//void gtk_application_init (GtkApplication *self);
void gtk_application_ready (GtkApplication *self);
void gtk_application_cleanup (GtkApplication *self);
//Accelerator functions
void gtk_application_set_use_quartz_accelerators(GtkApplication *self, 
						 gboolean use_accelerators);
gboolean gtk_application_use_quartz_accelerators(GtkApplication *self);
//Menu functions
void gtk_application_set_menu_bar       (GtkApplication *self, 
					 GtkMenuShell    *menu_shell);
void gtk_application_sync_menubar( void );
GtkApplicationMenuGroup * gtk_application_add_app_menu_group (GtkApplication*);
void gtk_application_add_app_menu_item   (GtkApplication *self,
					  GtkApplicationMenuGroup *group,
					  GtkMenuItem *menu_item);
//Dock Functions:

typedef enum {
  CRITICAL_REQUEST = 0,
  INFO_REQUEST = 10
} GtkApplicationAttentionType;

void gtk_application_set_dock_menu(GtkApplication *self, 
				   GtkMenuShell *menu_shell);
void gtk_application_set_dock_icon_pixbuf(GtkApplication *self,
					  GdkPixbuf *pixbuf);
void gtk_application_set_dock_icon_resource(GtkApplication *self,
					    const gchar  *name,
					    const gchar  *type,
					    const gchar  *subdir);
/** Ige-mac-dock provided two functions,
 * ige_mac_dock_set_overlay_from_pixbuf and
 * ige_mac_doc_set_overlay_from_resource, but OSX 10.5 and later do
 * not support application dock tile overlays. Document dock tiles
 * will by default represent a miniaturized view of the document's
 * contents badged with an even more miniaturized application
 * icon. The interface to change this is a bit complex and will be
 * left up to the application rather than implemented here.
 */
gint gtk_application_attention_request(GtkApplication *self,
				       GtkApplicationAttentionType type);
void gtk_application_cancel_attention_request(GtkApplication *self, gint id);

/* Bundle Functions */
/* ige-mac-bundle included a bunch of silly stuff for setting up the
 * environment. It's silly first because that's easier to do with a
 * startup script, and even easier to do with an LCEnvironment
 * dictionary in the bundle's Info.plist. 

 * Gtk applications, at least when launched with a shell script, still
 * return a bundle identifier and it's executable path is correct and
 * useful.  Ige-mac-bundle had a "is it an application bundle"
 * function, but NSBundle doesn't provide that; instead,
 * gtk_application_get_bundle_id will return NULL if it's not really a
 * bundle, there's no Info.plist, or if Info.plist doesn't have a
 * CFBundleIdentifier key (So if you need to detect being in a bundle,
 * make sure that your bundle has that key!) */
const gchar *gtk_application_get_bundle_path(GtkApplication *self);
const gchar *gtk_application_get_resource_path(GtkApplication *self);
const gchar *gtk_application_get_executable_path(GtkApplication *self);
const gchar *gtk_application_get_bundle_id(GtkApplication *self);
gpointer gtk_application_get_bundle_info(GtkApplication *self, const gchar *key);

//FIXME: These hard-coded functions should be replaced with a registry in GNSApplicationDelegate and GNSApplicationNotify

/* NSNotification responders: Override with something useful */
void gtk_application_activation_changed (GtkApplication *self, 
					 gboolean changed);

/* NSApplicationDelegate callbacks: Override these with real functions */
void gtk_application_should_load (GtkApplication *self, const gchar *utf8_path);
gboolean gtk_application_should_quit (GtkApplication *self);

G_END_DECLS

#endif /* __GTK_APPLICATION_H__ */
