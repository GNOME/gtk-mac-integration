#!/usr/bin/env python

import gtk
from gtk_osxapplication import *

class MainWindow(gtk.Window):
    def __init__(self, macapp):
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
        self.show()
#        menubar.hide()

        # Set up the menu bar integration
        macapp.set_menu_bar(menubar)

        # Take care of the Quit item, the integration code will put it
        # in the standard place
        quit_item.hide()

        # Add two groups with items in the application menu

        item = gtk.MenuItem("About")
        item.show()
        item.connect("activate", self.activate_cb)
        macapp.insert_app_menu_item(item, 0)
        item = gtk.MenuItem("Check for updates...")
        item.connect("activate", self.activate_cb)
        item.show()
        macapp.insert_app_menu_item(gtk.SeparatorMenuItem(), 1)
        macapp.insert_app_menu_item(item, 2)

        
        item = gtk.MenuItem("Preferences")
        item.connect("activate", self.activate_cb)
        item.show()
        macapp.insert_app_menu_item(item, 3)

#    def dock_clicked_cb(self, dock):
#        print "Dock clicked"

    def activate_cb(self, widget):
        try:
            print widget.child.get_text()
        except:
            print widget
            
if __name__ == '__main__':
    macapp = OSXApplication()
    window = MainWindow(macapp)
    window.connect("destroy", gtk.main_quit)
    macapp.ready()
#    window.show()

    gtk.main()

