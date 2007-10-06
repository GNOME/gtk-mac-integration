/* GTK+ Integration for the Mac OS X Menubar.
 *
 * Copyright (C) 2007 Pioneer Research Center USA, Inc.
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
#include <Carbon/Carbon.h>

#include "ige-mac-dock.h"

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

static GList *handlers;

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
