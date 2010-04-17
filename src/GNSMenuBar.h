/* GTK+ Integration with platform-specific application-wide features 
 * such as the OS X menubar and application delegate concepts.
 *
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
#import <Cocoa/Cocoa.h>
#include <gtk/gtk.h>
#include "gtkapplication.h"

/** GNSMenuBar is a wrapper class around NSMenu providing an extra
 * parameter for stashing the App menu groups.
 */

@interface GNSMenuBar : NSMenu
{
@private
  GList *app_menu_groups;
  GtkMenuBar *gtk_menubar;
}

/** Override the designated initializer */
- (id) initWithTitle: (NSString*) title;

/** Provide the initializer we actually want to use */
- (id) initWithGtkMenuBar: (GtkMenuBar*) menubar;

/** Create a new GtkApplicationMenuGroup, add it to the list, and
 * return a pointer to it.
 */
- (GtkApplicationMenuGroup *) addGroup;

/** Get a pointer to the current head of the app_menu_groups list
 */
- (GList *) app_menu_groups;

/** Resynchronize ourself with out GtkMenuBar */
-(void) resync;

/** Destructor */
- (void) dealloc;
@end
