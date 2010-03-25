AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS)

lib_LTLIBRARIES = libigemacintegration.la

libigemacintegration_la_SOURCES =			\
	GtkApplicationDelegate.h			\
	GtkApplicationDelegate.c			\
	GtkApplicationNotify.h				\
	GtkApplicationNotify.c				\
	GNSMenuItem.h					\
	GNSMenuItem.c					\
	getlabel.h					\
	getlabel.c					\
	cocoa_menu.h					\
	cocoa_menu.c					\
	cocoa_menu_item.h				\
	cocoa_menu_item.c				\
	gtkapplication_quartz.c				\
	gtkapplication.c				\
	gtkapplicationprivate.h				\
	ige-mac-dock.c					\
	ige-mac-bundle.c				\
	ige-mac-image-utils.c				\
	ige-mac-image-utils.h				\
	ige-mac-private.h				\
	$(integration_HEADERS)

libigemacintegration_la_CFLAGS = $(MAC_CFLAGS) -xobjective-c
libigemacintegration_la_OBJCFLAGS = $(MAC_CFLAGS)
libigemacintegration_la_LIBADD =  $(MAC_LIBS) -lobjc
libigemacintegration_la_LDFLAGS = -framework Carbon -framework ApplicationServices

integration_includedir = $(includedir)/igemacintegration
integration_include_HEADERS =				\
	ige-mac-integration.h				\
	gtkapplication.h				\
	ige-mac-dock.h					\
	ige-mac-bundle.h

# Test application
noinst_PROGRAMS = test-integration
test_integration_SOURCES = test-integration.c
test_integration_CFLAGS = $(MAC_CFLAGS)
test_integration_LDADD =  $(MAC_LIBS) libigemacintegration.la
