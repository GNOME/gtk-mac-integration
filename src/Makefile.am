AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS) -DLOCALEDIR=\"$(localedir)\"

-include $(INTROSPECTION_MAKEFILE)

SOURCES = \
	GtkApplicationDelegate.c		\
	GtkApplicationNotify.c			\
	GNSMenuBar.c					\
	GNSMenuItem.c					\
	GNSMenuDelegate.c				\
	getlabel.c						\
	cocoa_menu.c					\
	cocoa_menu_item.c				\
	gtkosxapplication_quartz.c		\
	gtkosxapplication.c				\
	gtkosx-image.c					\
	gtk-mac-image-utils.c

HEADER = \
	cocoa_menu.h              \
	cocoa_menu_item.h         \
	getlabel.h                \
	GNSMenuBar.h              \
	GNSMenuDelegate.h         \
	GNSMenuItem.h             \
	gtk-mac-image-utils.h     \
	gtk-mac-private.h         \
	GtkApplicationDelegate.h  \
	GtkApplicationNotify.h    \
	gtkosx-image.h            \
	gtkosxapplication.h       \
	gtkosxapplicationprivate.h

lib_LTLIBRARIES =
if WITH_GTK3
lib_LTLIBRARIES += libgtkmacintegration-gtk3.la
libgtkmacintegration_gtk3_la_SOURCES = $(SOURCES)
libgtkmacintegration_gtk3_la_CFLAGS = $(GTK3_CFLAGS) -xobjective-c
libgtkmacintegration_gtk3_la_OBJCFLAGS = $(GTK3_CFLAGS)
libgtkmacintegration_gtk3_la_LIBADD =  $(GTK3_LIBS) -lobjc
libgtkmacintegration_gtk3_la_LDFLAGS = -framework Carbon -framework ApplicationServices -version-info $(GTK_MAC_INTEGRATION_LT_VERSION)
endif
if WITH_GTK2
lib_LTLIBRARIES += libgtkmacintegration-gtk2.la
libgtkmacintegration_gtk2_la_SOURCES = $(SOURCES)
libgtkmacintegration_gtk2_la_CFLAGS = $(GTK2_CFLAGS) -xobjective-c
libgtkmacintegration_gtk2_la_OBJCFLAGS = $(GTK2_CFLAGS)
libgtkmacintegration_gtk2_la_LIBADD = $(GTK2_LIBS) -lobjc
libgtkmacintegration_gtk2_la_LDFLAGS = -framework Carbon -framework ApplicationServices -version-info $(GTK_MAC_INTEGRATION_LT_VERSION)
endif

integration_includedir = $(includedir)/gtkmacintegration
integration_include_HEADERS = gtkosxapplication.h
noinst_PROGRAMS =
test_integration_gtk3_CFLAGS =
if WITH_GTK3
noinst_PROGRAMS += test-integration-gtk3
test_integration_gtk3_SOURCES = test-integration.c
test_integration_gtk3_CFLAGS += $(GTK3_CFLAGS)
test_integration_gtk3_LDADD =  $(GTK3_LIBS) libgtkmacintegration-gtk3.la
endif
if WITH_GTK2
noinst_PROGRAMS += test-integration-gtk2
test_integration_gtk2_SOURCES = test-integration.c
test_integration_gtk2_CFLAGS = $(GTK2_CFLAGS)
test_integration_gtk2_LDADD =  $(GTK2_LIBS) libgtkmacintegration-gtk2.la
endif

EXTRA_DIST =  test-introspection.py testui.xml $(HEADER)

if HAVE_INTROSPECTION
if WITH_GTK3
test_integration_gtk3_CFLAGS += -DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_3_2

introspection_LIBS =
gir_INCLUDES = Atk-1.0 GLib-2.0
introspection_LIBS += libgtkmacintegration-gtk3.la
gir_INCLUDES += Gtk-3.0

GtkosxApplication-1.0.gir: $(INTROSPECTION_SCANNER) ${introspection_LIBS}

GtkosxApplication_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=Gtkosx \
	--warn-all

GtkosxApplication_1_0_gir_INCLUDES = ${gir_INCLUDES}

GtkosxApplication_1_0_gir_CFLAGS = \
		$(AM_CPPFLAGS)
GtkosxApplication_1_0_gir_LIBS = ${introspection_LIBS}
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
