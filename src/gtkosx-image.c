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

#include <config.h>
#include <gdk/gdkquartz.h> //for gdk_quartz_osx_version()
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "gtkosx-image.h"
#include "gtk-mac-image-utils.h"

/**
 * SECTION:gtkosx-image
 * @short_description: Convert image resourcess to NSImage
 *
 * Quartz requires icon resources for the dock or menu items to be
 * NSImages while Gnome uses either GdkPixbuf or image files such as
 * jpeg or png. The functions in this section convert those resources
 * into NSImage.
 */
/**
 * nsimage_from_resource:
 * @name: The filename
 * @type: The extension (e.g., jpg) of the filename
 * @subdir: The subdirectory of $Bundle/Contents/Resources in which to
 * look for the file.
 *
 * Retrieve an image file from the bundle and return an NSImage* of it.
 *
 * Returns: An autoreleased NSImage
 */
NSImage*
nsimage_from_resource (const gchar *name, const gchar* type, const gchar* subdir)
{
  NSString *ns_name, *ns_type, *ns_subdir, *path;
  NSImage *image = NULL;
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (type != NULL, NULL);
  g_return_val_if_fail (subdir != NULL, NULL);

  ns_name = [NSString stringWithUTF8String: name];
  ns_type = [NSString stringWithUTF8String: type];
  ns_subdir = [NSString stringWithUTF8String: subdir];
  path = [[NSBundle mainBundle] pathForResource: ns_name
	  ofType: ns_type inDirectory: ns_subdir];
  if (path)
    image = [[[NSImage alloc] initWithContentsOfFile: path] autorelease];

  return image;
}

/**
 * nsimage_from_pixbuf:
 * @pixbuf: The GdkPixbuf* to convert
 *
 * Create an NSImage from a CGImageRef.
 * Lifted from http://www.cocoadev.com/index.pl?CGImageRef
 *
 * Returns: An auto-released NSImage*
 */
NSImage*
nsimage_from_pixbuf (GdkPixbuf *pixbuf)
{
  CGImageRef image = NULL;
  NSRect imageRect = NSMakeRect (0.0, 0.0, 0.0, 0.0);
  CGContextRef imageContext = nil;
  NSImage* newImage = nil;

  g_return_val_if_fail (pixbuf !=  NULL, NULL);
  image = gtkosx_create_cgimage_from_pixbuf (pixbuf);
  // Get the image dimensions.
  imageRect.size.height = CGImageGetHeight (image);
  imageRect.size.width = CGImageGetWidth (image);

  // Create a new image to receive the Quartz image data.
  newImage = [[[NSImage alloc] initWithSize: imageRect.size] autorelease];
  [newImage lockFocus];

  // Get the Quartz context and draw.
#if MAC_OS_X_VERSION_MAX_ALLOWED < 101000
    imageContext = [[NSGraphicsContext currentContext] graphicsPort];
#else
# if MAC_OS_X_VERSION_MIN_REQUIRED < 101000
  if (gdk_quartz_osx_version () < GDK_OSX_YOSEMITE)
    imageContext = [[NSGraphicsContext currentContext] graphicsPort];
  else
#endif
    imageContext = [[NSGraphicsContext currentContext] CGContext];
#endif

  CGContextDrawImage (imageContext, * (CGRect*)&imageRect, image);
  [newImage unlockFocus];
  CGImageRelease (image);
  return newImage;
}
