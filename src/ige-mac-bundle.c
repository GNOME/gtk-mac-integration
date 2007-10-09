/* GTK+ Integration for the Mac OS X Bundle.
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

#include "ige-mac-bundle.h"

typedef struct IgeMacBundlePriv IgeMacBundlePriv;

struct IgeMacBundlePriv {
  gchar   *path;
  gchar   *id;
  UInt32   type;
  UInt32   creator;
};

static void   mac_bundle_finalize (GObject     *object);
static gchar *cf_string_to_utf8   (CFStringRef  str);

G_DEFINE_TYPE (IgeMacBundle, ige_mac_bundle, G_TYPE_OBJECT)

#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), IGE_TYPE_MAC_BUNDLE, IgeMacBundlePriv))

static void
ige_mac_bundle_class_init (IgeMacBundleClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = mac_bundle_finalize;

  g_type_class_add_private (object_class, sizeof (IgeMacBundlePriv));
}

static void
ige_mac_bundle_init (IgeMacBundle *bundle)
{
  IgeMacBundlePriv *priv = GET_PRIV (bundle);
  CFBundleRef       cf_bundle;
  CFURLRef          cf_url;
  CFStringRef       cf_string;

  cf_bundle = CFBundleGetMainBundle ();
  if (!cf_bundle)
    {
      g_warning ("Could not get main application bundle");
      return;
    }

  cf_url = CFBundleCopyBundleURL (cf_bundle);
  cf_string = CFURLCopyFileSystemPath (cf_url, kCFURLPOSIXPathStyle);
  priv->path = cf_string_to_utf8 (cf_string);
  CFRelease (cf_string);
  CFRelease (cf_url);

  g_print ("Bundle path: %s\n", priv->path);
  
  CFBundleGetPackageInfo (cf_bundle, &priv->type, &priv->creator);
  if (priv->type != 'APPL')
    g_warning ("The bundle is not an application bundle\n");

  cf_string = CFBundleGetIdentifier (cf_bundle);
  if (cf_string)
    priv->id = cf_string_to_utf8 (cf_string);
  else
    g_warning ("No bundle identifier found");

  g_print ("ID: %s\n", priv->id);
}

static void
mac_bundle_finalize (GObject *object)
{
  IgeMacBundlePriv *priv;

  priv = GET_PRIV (object);

  g_free (priv->path);
  g_free (priv->id);

  G_OBJECT_CLASS (ige_mac_bundle_parent_class)->finalize (object);
}

IgeMacBundle *
ige_mac_bundle_new (void)
{
  return g_object_new (IGE_TYPE_MAC_BUNDLE, NULL);
}

static gchar *
cf_string_to_utf8 (CFStringRef str)
{
  CFIndex  len;
  gchar   *ret;

  len = CFStringGetMaximumSizeForEncoding (CFStringGetLength (str), 
                                           kCFStringEncodingUTF8) + 1;

  ret = g_malloc (len);
  ret[len] = '\0';

  if (CFStringGetCString (str, ret, len, kCFStringEncodingUTF8))
    return ret;

  g_free (ret);
  return NULL;
}
