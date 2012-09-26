AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS)

-include $(INTROSPECTION_MAKEFILE)

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
	gtkosxapplication_quartz.c			\
	gtkosxapplication.c				\
	gtkosxapplicationprivate.h			\
	gtkosx-image.c					\
	gtkosx-image.h					\
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

if INTROSPECTION
if HAVE_INTROSPECTION
GtkosxApplication-1.0.gir: $(INTROSPECTION_SCANNER) libgtkmacintegration.la
GtkosxApplication_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=Gtkosx \
	--warn-all

GtkosxApplication_1_0_gir_INCLUDES = Atk-1.0 GLib-2.0 Gtk-3.0

GtkosxApplication_1_0_gir_CFLAGS = \
		$(AM_CPPFLAGS)
GtkosxApplication_1_0_gir_LIBS = libgtkmacintegration.la
GtkosxApplication_1_0_gir_FILES = \
	gtkosxapplication.h \
	gtkosxapplication.c

INTROSPECTION_GIRS = GtkosxApplication-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelibs_DATA)
endif
endif