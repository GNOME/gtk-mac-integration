/* GTK+ Integration for Mac OS X.
 *
 * Copyright (C) 2007, 2008 Imendio AB
 *
 * For further information, see:
 * http://sourceforge.net/apps/trac/gtk-osx/wiki/Integrate
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

#ifndef __GTK_MAC_PRIVATE_H__
#define __GTK_MAC_PRIVATE_H__

G_BEGIN_DECLS

gboolean _gtk_mac_menu_is_quit_menu_item_handled (void);
gboolean _gtk_mac_dock_is_quit_menu_item_handled (void);

G_END_DECLS

#endif /* __GTK_MAC_PRIVATE_H__ */
