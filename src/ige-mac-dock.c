/* GTK+ Integration for the Mac OS X Dock.
 *
 * Copyright (C) 2007 Imendio AB
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
#include <Carbon/Carbon.h>

#include "ige-mac-dock.h"
#include "ige-mac-bundle.h"
#include "ige-mac-image-utils.h"

enum {
  CLICKED,
  QUIT_ACTIVATE,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

typedef struct IgeMacDockPriv IgeMacDockPriv;

struct IgeMacDockPriv {
  glong id;
};

static void  mac_dock_finalize      (GObject          *object);
static OSErr mac_dock_handle_reopen (const AppleEvent *inAppleEvent,
                                     AppleEvent       *outAppleEvent,
                                     long              inHandlerRefcon);
static OSErr mac_dock_handle_quit   (const AppleEvent *inAppleEvent,
                                     AppleEvent       *outAppleEvent,
                                     long              inHandlerRefcon);

G_DEFINE_TYPE (IgeMacDock, ige_mac_dock, G_TYPE_OBJECT)

#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), IGE_TYPE_MAC_DOCK, IgeMacDockPriv))

static GList      *handlers;
static IgeMacDock *global_dock;

static void
ige_mac_dock_class_init (IgeMacDockClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = mac_dock_finalize;

  signals[CLICKED] =
    g_signal_new ("clicked",
                  IGE_TYPE_MAC_DOCK,
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  signals[QUIT_ACTIVATE] =
    g_signal_new ("quit-activate",
                  IGE_TYPE_MAC_DOCK,
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  g_type_class_add_private (object_class, sizeof (IgeMacDockPriv));
}

static void
ige_mac_dock_init (IgeMacDock *dock)
{
  IgeMacDockPriv *priv = GET_PRIV (dock);
  static glong    id;

  priv->id = ++id;

  handlers = g_list_prepend (handlers, dock);

  AEInstallEventHandler (kCoreEventClass, kAEReopenApplication, 
                         mac_dock_handle_reopen, priv->id, false);

  AEInstallEventHandler (kCoreEventClass, kAEQuitApplication, 
                         mac_dock_handle_quit, priv->id, false);
}

static void
mac_dock_finalize (GObject *object)
{
  IgeMacDockPriv *priv;

  priv = GET_PRIV (object);

  AERemoveEventHandler (kCoreEventClass, kAEReopenApplication,
                        mac_dock_handle_reopen, false);

  AERemoveEventHandler (kCoreEventClass, kAEQuitApplication,
                        mac_dock_handle_quit, false);

  handlers = g_list_remove (handlers, object);

  G_OBJECT_CLASS (ige_mac_dock_parent_class)->finalize (object);
}

IgeMacDock *
ige_mac_dock_new (void)
{
  return g_object_new (IGE_TYPE_MAC_DOCK, NULL);
}

IgeMacDock *
ige_mac_dock_get_default (void)
{
  if (!global_dock)
    global_dock = g_object_new (IGE_TYPE_MAC_DOCK, NULL);

  return global_dock;
}

static IgeMacDock *
mac_dock_get_from_id (gulong id)
{
  GList      *l;
  IgeMacDock *dock = NULL;

  for (l = handlers; l; l = l->next)
    {
      dock = l->data;
      if (GET_PRIV (dock)->id == id)
        break;

      dock = NULL;
  }

  return dock;
}

static OSErr
mac_dock_handle_reopen (const AppleEvent *inAppleEvent, 
                        AppleEvent       *outAppleEvent, 
                        long              inHandlerRefcon)
{
  IgeMacDock *dock;

  dock = mac_dock_get_from_id (inHandlerRefcon);

  if (dock)
    g_signal_emit (dock, signals[CLICKED], 0);
  
  return noErr;
}

static OSErr
mac_dock_handle_quit (const AppleEvent *inAppleEvent, 
                      AppleEvent       *outAppleEvent, 
                      long              inHandlerRefcon)
{
  IgeMacDock *dock;

  dock = mac_dock_get_from_id (inHandlerRefcon);

  if (dock)
    g_signal_emit (dock, signals[QUIT_ACTIVATE], 0);

  return noErr;
}

void
ige_mac_dock_set_icon_from_pixbuf (IgeMacDock *dock,
                                   GdkPixbuf  *pixbuf)
{
  if (!pixbuf)
    RestoreApplicationDockTileImage ();
  else
    {
      CGImageRef image;

      image = ige_mac_image_from_pixbuf (pixbuf);
      SetApplicationDockTileImage (image);
      CGImageRelease (image);
    }
}

void
ige_mac_dock_set_icon_from_resource (IgeMacDock   *dock,
                                     IgeMacBundle *bundle,
                                     const gchar  *name,
                                     const gchar  *type,
                                     const gchar  *subdir)
{
  gchar *path;

  g_return_if_fail (IGE_IS_MAC_DOCK (dock));
  g_return_if_fail (name != NULL);

  path = ige_mac_bundle_get_resource_path (bundle, name, type, subdir);
  if (path)
    {
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_file (path, NULL);
      if (pixbuf)
        {
          ige_mac_dock_set_icon_from_pixbuf (dock, pixbuf);
          g_object_unref (pixbuf);
        }

      g_free (path);
    }
}

void
ige_mac_dock_set_overlay_from_pixbuf (IgeMacDock  *dock,
                                      GdkPixbuf   *pixbuf)
{
  CGImageRef image;

  g_return_if_fail (IGE_IS_MAC_DOCK (dock));
  g_return_if_fail (GDK_IS_PIXBUF (pixbuf));

  image = ige_mac_image_from_pixbuf (pixbuf);
  OverlayApplicationDockTileImage (image);
  CGImageRelease (image);
}

void
ige_mac_dock_set_overlay_from_resource (IgeMacDock   *dock,
                                        IgeMacBundle *bundle,
                                        const gchar  *name,
                                        const gchar  *type,
                                        const gchar  *subdir)
{
  gchar *path;

  g_return_if_fail (IGE_IS_MAC_DOCK (dock));
  g_return_if_fail (name != NULL);

  path = ige_mac_bundle_get_resource_path (bundle, name, type, subdir);
  if (path)
    {
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_file (path, NULL);
      if (pixbuf)
        {
          ige_mac_dock_set_overlay_from_pixbuf (dock, pixbuf);
          g_object_unref (pixbuf);
        }

      g_free (path);
    }
}

struct _IgeMacAttentionRequest {
  NMRec    nm_request;
  guint    timeout_id;
  gboolean is_cancelled;
};

static gboolean
mac_dock_attention_cb (IgeMacAttentionRequest *request)
{
  request->timeout_id = 0;
  request->is_cancelled = TRUE;

  NMRemove (&request->nm_request);

  return FALSE;
}


/* FIXME: Add listener for "application activated" and cancel any
 * requests.
 */
IgeMacAttentionRequest *
ige_mac_dock_attention_request (IgeMacDock              *dock, 
                                IgeMacAttentionType  type)
{
  IgeMacAttentionRequest *request;

  request = g_new0 (IgeMacAttentionRequest, 1);

  request->nm_request.nmMark = 1;
  request->nm_request.qType = nmType;
  
  if (NMInstall (&request->nm_request) != noErr)
    {
      g_free (request);
      return NULL;
    }

  if (type == IGE_MAC_ATTENTION_INFO)
    request->timeout_id = gdk_threads_add_timeout (1000, 
                                                   (GSourceFunc) mac_dock_attention_cb, 
                                                   request);

  return request;
}

void
ige_mac_dock_attention_cancel (IgeMacDock                 *dock, 
                               IgeMacAttentionRequest *request)
{
  if (request->timeout_id)
    g_source_remove (request->timeout_id);

  if (!request->is_cancelled)
    NMRemove (&request->nm_request);

  g_free (request);
}
