/*
 * Copyright (C) 2007 Imendio AB
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

/* This interface is checked to be 64-bit safe */

#include <config.h>
#include <gtk/gtk.h>
#include <Carbon/Carbon.h>

#include "gtk-mac-image-utils.h"

CGImageRef
gtkosx_create_cgimage_from_pixbuf (GdkPixbuf *pixbuf)
{
  CGColorSpaceRef   colorspace;
  CGDataProviderRef data_provider;
  CGBitmapInfo      bmi;
  CGImageRef        image;
  void             *data;
  gint              rowstride;
  gint              pixbuf_width, pixbuf_height;
  gboolean          has_alpha;

  pixbuf_width = gdk_pixbuf_get_width (pixbuf);
  pixbuf_height = gdk_pixbuf_get_height (pixbuf);
  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  has_alpha = gdk_pixbuf_get_has_alpha (pixbuf);

  data = gdk_pixbuf_get_pixels (pixbuf);

  colorspace = CGColorSpaceCreateDeviceRGB ();
  data_provider = CGDataProviderCreateWithData (NULL, data,
                  pixbuf_height * rowstride,
                  NULL);
  bmi = kCGBitmapByteOrderDefault |
    (CGBitmapInfo)(has_alpha ? kCGImageAlphaLast : kCGImageAlphaNone);

  image = CGImageCreate (pixbuf_width, pixbuf_height, 8,
                         has_alpha ? 32 : 24, rowstride,
                         colorspace,
                         bmi,
                         data_provider, NULL, FALSE,
                         kCGRenderingIntentDefault);

  CGDataProviderRelease (data_provider);
  CGColorSpaceRelease (colorspace);

  return image;
}
