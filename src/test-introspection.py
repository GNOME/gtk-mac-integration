#!/usr/bin/env python

from gi import require_version
require_version('Gtk', '2.0')
from gi.repository import Gtk
from gi.repository import GObject
require_version('GtkosxApplication', '1.0')
from gi.repository import GtkosxApplication

class MainWindow(Gtk.Window):
    def __init__(self, macapp):
        Gtk.Window.__init__(self)

        self.set_default_size(400, 300)

        vbox = Gtk.VBox(homogeneous = False, spacing = 0)
        self.add(vbox)

        vbox.pack_start(Gtk.Label("Some content here"), True, True, 0)

        # Setup a menu bar with GTK+
        menubar = Gtk.MenuBar()

        menu = Gtk.Menu()
        item = Gtk.MenuItem.new_with_label("Open")
        item.connect("activate", self.activate_cb)
        menu.add(item)

        item = Gtk.MenuItem.new_with_label("Save")
        item.connect("activate", self.activate_cb)
        menu.add(item)

        quit_item = Gtk.MenuItem.new_with_label("Quit")
        quit_item.connect("activate", lambda d: Gtk.main_quit())
        menu.add(quit_item)

        item = Gtk.MenuItem.new_with_label("File")
        item.set_submenu(menu)
        menubar.add(item)

        menubar.show_all()

        vbox.pack_start(menubar, True, True, 0)
        self.show()
#        menubar.hide()

        # Set up the menu bar integration
        macapp.set_menu_bar(menubar)

        # Take care of the Quit item, the integration code will put it
        # in the standard place
        quit_item.hide()

        # Add two groups with items in the application menu

        item = Gtk.MenuItem.new_with_label("About")
        item.show()
        item.connect("activate", self.activate_cb)
        macapp.insert_app_menu_item(item, 0)
        item = Gtk.MenuItem.new_with_label("Check for updates...")
        item.connect("activate", self.activate_cb)
        item.show()
        macapp.insert_app_menu_item(Gtk.SeparatorMenuItem(), 1)
        macapp.insert_app_menu_item(item, 2)

        
        item = Gtk.MenuItem.new_with_label("Preferences")
        item.connect("activate", self.activate_cb)
        item.show()
        macapp.insert_app_menu_item(item, 3)

#    def dock_clicked_cb(self, dock):
#        print("Dock clicked")

    def activate_cb(self, widget):
        try:
            print(widget.get_child().get_text())
        except:
            print(widget)
            
if __name__ == '__main__':
    macapp = GtkosxApplication.Application()
    window = MainWindow(macapp)
    window.connect("destroy", Gtk.main_quit)
    macapp.ready()
#Just to illustrate using the qtkosx_application_get... functions
    print(macapp.get_resource_path())
    Gtk.main()

