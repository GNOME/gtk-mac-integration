AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS)

lib_LTLIBRARIES = libigemacintegration.la

libigemacintegration_la_SOURCES =			\
	ige-mac-menu.c					\
	ige-mac-dock.c					\
	ige-mac-bundle.c				\
	$(integration_HEADERS)

libigemacintegration_la_CFLAGS = $(MAC_CFLAGS)
libigemacintegration_la_LIBADD =  $(MAC_LIBS)
libigemacintegration_la_LDFLAGS = -framework Carbon

integration_includedir = $(includedir)/igemacintegration
integration_include_HEADERS =				\
	ige-mac-menu.h					\
	ige-mac-dock.h					\
	ige-mac-bundle.h

# Test application
noinst_PROGRAMS = test-integration
test_integration_SOURCES = test-integration.c
test_integration_CFLAGS = $(MAC_CFLAGS)
test_integration_LDADD =  $(MAC_LIBS) libigemacintegration.la