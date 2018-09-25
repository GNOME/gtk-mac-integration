EXTRA_DIST = gtk-mac-integration-gtk3.pc.in gtk-mac-integration-gtk2.pc.in

pkgconfig_DATA = gtk-mac-integration.pc

if WITH_GTK3
pkgconfig_DATA += gtk-mac-integration-gtk3.pc
if !WITH_GTK2
default_pc_file = gtk-mac-integration-gtk3.pc
endif
endif

if WITH_GTK2
pkgconfig_DATA += gtk-mac-integration-gtk2.pc
default_pc_file = gtk-mac-integration-gtk2.pc
endif

gtk-mac-integration.pc: ${default_pc_file}
	cp ${default_pc_file} $@

DISTCLEANFILES=gtk-mac-integration.pc
