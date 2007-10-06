#!/usr/bin/env python

import gtk
from igemacintegration import *

class MainWindow(gtk.Window):
    def __init__(self):
        gtk.Window.__init__(self)

        self.set_default_size(400, 300)

        vbox = gtk.VBox(False, 0)
        self.add(vbox)

        vbox.pack_start(gtk.Label("Some content here"), True, True, 0)

        # Setup a menu bar with GTK+
        menubar = gtk.MenuBar()

        menu = gtk.Menu()
        item = gtk.MenuItem("Open")
        item.connect("activate", self.activate_cb)
        menu.add(item)

        item = gtk.MenuItem("Save")
        item.connect("activate", self.activate_cb)
        menu.add(item)

        quit_item = gtk.MenuItem("Quit")
        quit_item.connect("activate", lambda d: gtk.main_quit())
        menu.add(quit_item)

        item = gtk.MenuItem("File")
        item.set_submenu(menu)
        menubar.add(item)

        menubar.show_all()

        vbox.pack_start(menubar)
        menubar.hide()

        # Set up the menu bar integration
        macmenu = MacMenu()
        macmenu.set_menu_bar(menubar)

        # Take care of the Quit item, the integration code will put it
        # in the standard place
        macmenu.set_quit_menu_item(quit_item)

        # Add two groups with items in the application menu
        group = macmenu.add_app_menu_group()
        item = gtk.MenuItem("About")
        item.connect("activate", self.activate_cb)
        group.add_app_menu_item(item, None)
        item = gtk.MenuItem("Check for updates...")
        item.connect("activate", self.activate_cb)
        group.add_app_menu_item(item, None)

        group = macmenu.add_app_menu_group()
        item = gtk.MenuItem("Preferences")
        item.connect("activate", self.activate_cb)
        group.add_app_menu_item(item, None)
        
        # Set up the dock integration
        dock = MacDock()
        dock.connect('quit-activate', lambda d: gtk.main_quit())
        dock.connect('clicked', self.dock_clicked_cb)

        # Keep the reference so it's not GC:ed.
        self.dock = dock
        
    def dock_clicked_cb(self, dock):
        print "Dock clicked"

    def activate_cb(self, widget):
        try:
            print widget.child.get_text()
        except:
            print widget
            
if __name__ == '__main__':
    window = MainWindow()
    window.connect("destroy", gtk.main_quit)
    window.show()

    gtk.main()

