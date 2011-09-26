AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS)

lib_LTLIBRARIES = libgtkmacintegration.la

libgtkmacintegration_la_SOURCES =			\
	GtkApplicationDelegate.h			\
	GtkApplicationDelegate.c			\
	GtkApplicationNotify.h				\
	GtkApplicationNotify.c				\
	GNSMenuBar.h					\
	GNSMenuBar.c					\
	GNSMenuItem.h					\
	GNSMenuItem.c					\
	getlabel.h					\
	getlabel.c					\
	cocoa_menu.h					\
	cocoa_menu.c					\
	cocoa_menu_item.h				\
	cocoa_menu_item.c				\
	gtkosxapplication_quartz.c				\
	gtkosxapplication.c				\
	gtkosxapplicationprivate.h				\
	gtk-mac-dock.c					\
	gtk-mac-bundle.c				\
	gtk-mac-menu.c					\
	gtk-mac-image-utils.c				\
	gtk-mac-image-utils.h				\
	gtk-mac-private.h				\
	$(integration_HEADERS)

libgtkmacintegration_la_CFLAGS = $(MAC_CFLAGS) -xobjective-c
libgtkmacintegration_la_OBJCFLAGS = $(MAC_CFLAGS)
libgtkmacintegration_la_LIBADD =  $(MAC_LIBS) -lobjc
libgtkmacintegration_la_LDFLAGS = -framework Carbon -framework ApplicationServices

integration_includedir = $(includedir)/gtkmacintegration
integration_include_HEADERS =				\
	gtk-mac-integration.h				\
	gtkosxapplication.h				\
	gtk-mac-menu.h					\
	gtk-mac-dock.h					\
	gtk-mac-bundle.h

# Test application
noinst_PROGRAMS = test-integration
test_integration_SOURCES = test-integration.c
test_integration_CFLAGS = $(MAC_CFLAGS)
test_integration_LDADD =  $(MAC_LIBS) libgtkmacintegration.la

EXTRA_DIST = testui.xml
