AM_CPPFLAGS = -I$(top_srcdir) $(WARN_CFLAGS)

lib_LTLIBRARIES = libigemacmenu.la

libigemacmenu_la_SOURCES = 		\
	ige-mac-menu.c			\
	$(menu_HEADERS)

libigemacmenu_la_CFLAGS = $(MENU_CFLAGS)
libigemacmenu_la_LIBADD =  $(MENU_LIBS)
libigemacmenu_la_LDFLAGS = -framework Carbon

menu_includedir = $(includedir)/igemacmenu
menu_include_HEADERS = 			\
	ige-mac-menu.h

# Test application
noinst_PROGRAMS = test-menu
test_menu_SOURCES = test-menu.c
test_menu_CFLAGS = $(MENU_CFLAGS)
test_menu_LDADD =  $(MENU_LIBS) libigemacmenu.la
