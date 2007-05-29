#include <gtk/gtk.h>
#include <glib.h>
#include <Carbon/Carbon.h>

/* TODO
 *
 * - Setup shortcuts, possibly transforming ctrl->cmd
 * - Sync menus
 * - Create on demand? (can this be done with gtk+? ie fill in menu items when the menu is opened)
 * - Figure out what to do per app/window...
 * - Toggle/radio items
 *
 */

#define GTK_QUARTZ_MENU_CREATOR 'GTKC'
#define GTK_QUARTZ_ITEM_WIDGET  'GWID'

static OSStatus
menu_event_handler_func (EventHandlerCallRef  event_handler_call_ref, 
			 EventRef             event_ref, 
			 void                *data)
{
  UInt32 event_class = GetEventClass (event_ref);
  UInt32 event_kind = GetEventKind (event_ref);
  MenuRef menu_ref;

  switch (event_class) 
    {
    case kEventClassCommand:
      /* This is called when activating (is that the right GTK+ term?)
       * a menu item.
       */
      if(event_kind == kEventCommandProcess)
	{
	  HICommand command;
	  OSStatus  err;

	  //g_print ("Menu: kEventClassCommand/kEventCommandProcess\n");

	  err = GetEventParameter (event_ref, kEventParamDirectObject, 
				   typeHICommand, 0, 
				   sizeof (command), 0, &command);

	  if (err == noErr)
	    {
	      GtkWidget *widget = NULL;
              
	      if (command.commandID == kHICommandQuit) {
                gtk_main_quit (); /* Just testing... */
                return noErr;
	      }
	      
	      /* Get any GtkWidget associated with the item. */
	      err = GetMenuItemProperty (command.menu.menuRef, 
					 command.menu.menuItemIndex, 
					 GTK_QUARTZ_MENU_CREATOR, 
					 GTK_QUARTZ_ITEM_WIDGET,
					 sizeof (widget), 0, &widget);
	      if (err == noErr && widget)
		{
		  gtk_menu_item_activate (GTK_MENU_ITEM (widget));
		  return noErr;
		}
	    }
	}
      break;

    case kEventClassMenu: 
        GetEventParameter (event_ref, 
			   kEventParamDirectObject, 
			   typeMenuRef, 
			   NULL, 
			   sizeof (menu_ref), 
			   NULL, 
			   &menu_ref);

        switch (event_kind)
	  {
	  case kEventMenuTargetItem:
	    /* This is called when an item is selected (what is the
	     * GTK+ term? prelight?)
	     */
	    //g_print ("kEventClassMenu/kEventMenuTargetItem\n");
	    break;

	  case kEventMenuOpening:
	    /* Is it possible to dynamically build the menu here? We
	     * can at least set visibility/sensitivity. 
	     */
	    //g_print ("kEventClassMenu/kEventMenuOpening\n");
	    break;
	    
	  case kEventMenuClosed:
	    //g_print ("kEventClassMenu/kEventMenuClosed\n");
	    break;

	  default:
	    break;
	  }

	break;
	
    default:
      break;
    }

  return CallNextEventHandler (event_handler_call_ref, event_ref);
}

static void
setup_menu_event_handler (void)
{
  EventHandlerUPP menu_event_handler_upp;
  EventHandlerRef menu_event_handler_ref;
  const EventTypeSpec menu_events[] = {
    { kEventClassCommand, kEventCommandProcess },
    { kEventClassMenu, kEventMenuTargetItem },
    { kEventClassMenu, kEventMenuOpening },
    { kEventClassMenu, kEventMenuClosed }
  };

  /* FIXME: We might have to install one per window? */

  menu_event_handler_upp = NewEventHandlerUPP (menu_event_handler_func);
  InstallEventHandler (GetApplicationEventTarget (), menu_event_handler_upp,
		       GetEventTypeCount (menu_events), menu_events, 0,
		       &menu_event_handler_ref);
  
#if 0
  /* FIXME: Remove the handler with: */
  RemoveEventHandler(menu_event_handler_ref);
  DisposeEventHandlerUPP(menu_event_handler_upp);
#endif
}

static GtkWidget *
find_menu_label (GtkWidget *widget)
{
  GList     *children, *l;
  GtkWidget *label;
  
  if (GTK_IS_LABEL (widget))
    return widget;
  
  label = NULL;
  
  if (GTK_IS_CONTAINER (widget))
    {
      children = gtk_container_get_children (GTK_CONTAINER (widget));

      for (l = children; l; l = l->next)
	{
	  label = find_menu_label (l->data);
	  if (label)
	    break;
	}
      
      g_list_free (children);
    }

  return label;
}

static const gchar *
get_menu_label_text (GtkMenuItem *item)
{
  GtkWidget *label;
  gchar     *text;
  
  label = find_menu_label (GTK_WIDGET (item));  
  if (!label)
    return NULL;

  gtk_label_get (GTK_LABEL (label), &text);
  return text;
}

static void
sync_menu_shell (GtkMenuShell *menu_shell,
		 MenuRef       carbon_menu)
{
  GList *children, *l;

  children = gtk_container_get_children (GTK_CONTAINER (menu_shell));
  for (l = children; l; l = l->next)
    {
      const gchar   *label;
      CFStringRef    cfstr;
      GtkWidget     *submenu;
      MenuItemIndex  index;
      
      label = get_menu_label_text (l->data);
      if (label)
	cfstr = CFStringCreateWithCString (NULL, label, kCFStringEncodingUTF8);
      else
	cfstr = NULL;

      submenu = gtk_menu_item_get_submenu (l->data);
      if (submenu)
	{
	  MenuRef carbon_submenu;
  
	  CreateNewMenu (0, 0, &carbon_submenu);
	  SetMenuTitleWithCFString (carbon_submenu, cfstr);
	  AppendMenuItemTextWithCFString (carbon_menu, NULL, 0, 0, &index);
	  SetMenuItemProperty (carbon_menu, index, 
			       GTK_QUARTZ_MENU_CREATOR, 
			       GTK_QUARTZ_ITEM_WIDGET, 
			       sizeof (l->data), &l->data);
	  
	  SetMenuItemHierarchicalMenu (carbon_menu, index, carbon_submenu);

	  sync_menu_shell (GTK_MENU_SHELL (submenu), carbon_submenu);
	}
      else if (GTK_IS_SEPARATOR_MENU_ITEM (l->data))
	{
	  AppendMenuItemTextWithCFString (carbon_menu, NULL, kMenuItemAttrSeparator, 0, &index);
	  SetMenuItemProperty(carbon_menu, index, 
			      GTK_QUARTZ_MENU_CREATOR, 
			      GTK_QUARTZ_ITEM_WIDGET, 
			      sizeof (l->data), &l->data);
	}
      else
	{
	  AppendMenuItemTextWithCFString (carbon_menu, cfstr, 0, 0, &index);
	  SetMenuItemProperty (carbon_menu, index, 
			       GTK_QUARTZ_MENU_CREATOR, 
			       GTK_QUARTZ_ITEM_WIDGET,
			       sizeof (l->data), &l->data);

	  /* Setting a shortcut:
	     SetMenuItemCommandKey (carbon_menu, index, false, 'A'); 
	     or:
	     SetItemCmd (carbon_menu, index, 'A');
	     SetMenuItemModifiers (carbon_menu, index, kMenuNoCommandModifier);
	     SetMenuItemCommandID (carbon_menu, index, 'Boom');
	  */
	}

      if (!GTK_WIDGET_IS_SENSITIVE (l->data))
	ChangeMenuItemAttributes (carbon_menu, index, kMenuItemAttrDisabled, 0);

      if (!GTK_WIDGET_VISIBLE (l->data))
	ChangeMenuItemAttributes (carbon_menu, index, kMenuItemAttrHidden, 0);

      if (cfstr)
	CFRelease (cfstr);
    }
  g_list_free (children);
}

void
sync_menu_takeover_menu (GtkWidget *menu)
{
   MenuRef carbon_menubar;

  CreateNewMenu (0 /*id*/, 0 /*options*/, &carbon_menubar);
  SetRootMenu (carbon_menubar);

  setup_menu_event_handler ();
  
  sync_menu_shell (GTK_MENU_SHELL (menu), carbon_menubar);

  /*gtk_widget_hide (menu);*/
}
