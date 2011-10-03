/* GTK+ Integration for app bundles.
 *
 * Copyright (C) 2007-2008 Imendio AB
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

/* TODO: Add command line parsing and remove any
 * -psn_... arguments?
 */

#ifndef __x86_64__
#include <gtk/gtk.h>
#include <Carbon/Carbon.h>

#include "gtk-mac-bundle.h"

typedef struct GtkMacBundlePriv GtkMacBundlePriv;

struct GtkMacBundlePriv {
  CFBundleRef  cf_bundle; 
  gchar       *path;
  gchar       *id;
  gchar       *datadir;
  gchar       *localedir;
  UInt32       type;
  UInt32       creator;
};

static void   mac_bundle_finalize              (GObject      *object);
static gchar *cf_string_to_utf8                (CFStringRef   str);
static void   mac_bundle_set_environment_value (GtkMacBundle *bundle,
                                                const gchar  *key,
                                                const gchar  *value);

static GtkMacBundle *global_bundle;

G_DEFINE_TYPE (GtkMacBundle, gtk_mac_bundle, G_TYPE_OBJECT)

#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_MAC_BUNDLE, GtkMacBundlePriv))

static void
gtk_mac_bundle_class_init (GtkMacBundleClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = mac_bundle_finalize;

  g_type_class_add_private (object_class, sizeof (GtkMacBundlePriv));
}

static void
gtk_mac_bundle_init (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);
  CFURLRef          cf_url;
  CFStringRef       cf_string;
  CFDictionaryRef   cf_dict;

  priv->cf_bundle = CFBundleGetMainBundle ();
  if (!priv->cf_bundle)
    return;

  CFRetain (priv->cf_bundle);

  /* Bundle or binary location. */
  cf_url = CFBundleCopyBundleURL (priv->cf_bundle);
  cf_string = CFURLCopyFileSystemPath (cf_url, kCFURLPOSIXPathStyle);
  priv->path = cf_string_to_utf8 (cf_string);
  CFRelease (cf_string);
  CFRelease (cf_url);

  /* Package info. */
  CFBundleGetPackageInfo (priv->cf_bundle, &priv->type, &priv->creator);

  /* Identifier. */
  cf_string = CFBundleGetIdentifier (priv->cf_bundle);
  if (cf_string)
    priv->id = cf_string_to_utf8 (cf_string);

  /* Get non-localized keys. */
  cf_dict = CFBundleGetInfoDictionary (priv->cf_bundle);
  if (cf_dict)
    {
      CFDictionaryRef   env_dict;
      CFIndex           n_keys, i;
      const void      **keys;
      const void      **values;

      env_dict = (CFDictionaryRef) CFDictionaryGetValue (cf_dict, CFSTR ("LSEnvironment"));
      if (env_dict)
        {
          n_keys = CFDictionaryGetCount (env_dict);

          keys = (const void **) g_new (void *, n_keys);
          values = (const void **) g_new (void *, n_keys);

          CFDictionaryGetKeysAndValues (env_dict, keys, values);

          for (i = 0; i < n_keys; i++)
            {
              gchar *key;
              gchar *value;

              key = cf_string_to_utf8 ((CFStringRef) keys[i]);
              value = cf_string_to_utf8 ((CFStringRef) values[i]);

              mac_bundle_set_environment_value (bundle, key, value);

              g_free (key);
              g_free (value);
            }

          g_free (keys);
          g_free (values);
        }      
    }
}

static void
mac_bundle_finalize (GObject *object)
{
  GtkMacBundlePriv *priv;

  priv = GET_PRIV (object);

  g_free (priv->path);
  g_free (priv->id);
  g_free (priv->datadir);
  g_free (priv->localedir);

  CFRelease (priv->cf_bundle);

  G_OBJECT_CLASS (gtk_mac_bundle_parent_class)->finalize (object);
}

GtkMacBundle *
gtk_mac_bundle_new (void)
{
  return g_object_new (GTK_TYPE_MAC_BUNDLE, NULL);
}

GtkMacBundle *
gtk_mac_bundle_get_default (void)
{
  if (!global_bundle)
    global_bundle = gtk_mac_bundle_new ();

  return global_bundle;
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

static void
mac_bundle_set_environment_value (GtkMacBundle *bundle,
                                  const gchar  *key, 
                                  const gchar  *value)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);
  GRegex           *regex;
  gchar            *new_value;

  regex = g_regex_new ("@executable_path", 0, 0, NULL);

  new_value = g_regex_replace_literal (regex,
                                       value,
                                       -1,
                                       0,
                                       priv->path,
                                       0, NULL);

  g_print ("%s => %s\n", value, new_value);

  if (new_value)
    value = new_value;


  g_setenv (key, value, TRUE);

  g_free (new_value);
  g_regex_unref (regex);
}

const gchar *
gtk_mac_bundle_get_id (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);

  return priv->id;
}

const gchar *
gtk_mac_bundle_get_path (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);

  return priv->path;
}

gboolean
gtk_mac_bundle_get_is_app_bundle (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);

  return (priv->type == 'APPL' && priv->id);
}

const gchar *
gtk_mac_bundle_get_datadir (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);

  if (!gtk_mac_bundle_get_is_app_bundle (bundle))
    return NULL;

  if (!priv->datadir)
    {
      priv->datadir = g_build_filename (priv->path,
                                        "Contents",
                                        "Resources",
                                        "share",
                                        NULL);
    }

  return priv->datadir;
}

const gchar *
gtk_mac_bundle_get_localedir (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);

  if (!gtk_mac_bundle_get_is_app_bundle (bundle))
    return NULL;

  if (!priv->localedir)
    {
      priv->localedir = g_build_filename (priv->path,
                                          "Contents",
                                          "Resources",
                                          "share",
                                          "locale",
                                          NULL);
    }

  return priv->localedir;
}

void
gtk_mac_bundle_setup_environment (GtkMacBundle *bundle)
{
  GtkMacBundlePriv *priv = GET_PRIV (bundle);
  gchar            *resources;
  gchar            *share, *lib, *etc;
  gchar            *etc_xdg, *etc_immodules, *etc_gtkrc;
  gchar            *etc_pixbuf, *etc_pangorc;
  const gchar      *rc_files;

  if (!gtk_mac_bundle_get_is_app_bundle (bundle))
    return;

  resources = g_build_filename (priv->path,
                                "Contents",
                                "Resources",
                                NULL);

  share = g_build_filename (resources, "share", NULL);
  lib = g_build_filename (resources, "lib", NULL);
  etc = g_build_filename (resources, "etc", NULL);
  etc_xdg = g_build_filename (etc, "xdg", NULL);
  etc_immodules = g_build_filename (etc, "gtk-2.0", "gtk.immodules", NULL);
  etc_gtkrc = g_build_filename (etc, "gtk-2.0", "gtkrc", NULL);
  etc_pixbuf = g_build_filename (etc, "gtk-2.0", "gdk-pixbuf.loaders", NULL);
  etc_pangorc = g_build_filename (etc, "pango", "pangorc", NULL);

  g_setenv ("XDG_CONFIG_DIRS", etc_xdg, TRUE);
  g_setenv ("XDG_DATA_DIRS", share, TRUE);
  g_setenv ("GTK_DATA_PREFIX", share, TRUE);
  g_setenv ("GTK_EXE_PREFIX", resources, TRUE);
  g_setenv ("GTK_PATH_PREFIX", resources, TRUE);

  /* Append the normal gtkrc path to allow customizing the theme from
   * Info.plist.
   */
  rc_files = g_getenv ("GTK2_RC_FILES");
  if (rc_files)
    {
      gchar *tmp;

      tmp = g_strdup_printf ("%s:%s", rc_files, etc_gtkrc);
      g_setenv ("GTK2_RC_FILES", tmp, TRUE);
      g_free (tmp);
    }
  else
    g_setenv ("GTK2_RC_FILES", etc_gtkrc, TRUE);

  g_setenv ("GTK_IM_MODULE_FILE", etc_immodules, TRUE);
  g_setenv ("GDK_PIXBUF_MODULE_FILE", etc_pixbuf, TRUE);
  g_setenv ("PANGO_RC_FILE", etc_pangorc, TRUE);
  g_setenv ("CHARSETALIASDIR", lib, TRUE);

  // could add FONTCONFIG_FILE

  /*export LANG="\`grep \"\\\`defaults read .GlobalPreferences AppleCollationOrder \
 2>&1\\\`_\" /usr/share/locale/locale.alias | tail -n1 | sed 's/\./ /' | \
 awk '{print \$2}'\`.UTF-8"*/

  g_free (share);
  g_free (lib);
  g_free (etc);
  g_free (etc_xdg);
  g_free (etc_immodules);
  g_free (etc_gtkrc);
  g_free (etc_pixbuf);
  g_free (etc_pangorc);
}

gchar *
gtk_mac_bundle_get_resource_path (GtkMacBundle *bundle,
                                  const gchar  *name,
                                  const gchar  *type,
                                  const gchar  *subdir)
{
  GtkMacBundlePriv *priv;
  CFURLRef          cf_url;
  CFStringRef       cf_string;
  gchar            *path;

  if (!bundle)
    bundle = gtk_mac_bundle_get_default ();

  priv = GET_PRIV (bundle);

  if (!priv->cf_bundle)
    return NULL;

  // FIXME: Look at using CFURLGetFileSystemRepresentation (urlcf_, true, (UInt8*)outPathName, 256)

  // FIXME: crate real cfstring here...
  cf_url = CFBundleCopyResourceURL (priv->cf_bundle, 
                                    CFSTR("name"), CFSTR("type"), CFSTR("subdir"));
  cf_string = CFURLCopyFileSystemPath (cf_url, kCFURLPOSIXPathStyle);
  path = cf_string_to_utf8 (cf_string);
  CFRelease (cf_string);
  CFRelease (cf_url);

  return path;
}

#endif //__x86_64__
