INCLUDES = -I$(top_srcdir)/../src $(PYTHON_INCLUDES)

igemacmenudir = $(libdir)/python$(PYTHON_VERSION)/site-packages

igemacmenu_LTLIBRARIES = igemacmenu.la

igemacmenu.defs: $(top_srcdir)/src/ige-mac-menu.h
	$(PYTHON) $(datadir)/pygtk/2.0/codegen/h2def.py $< > $@

igemacmenu.c: igemacmenu.defs igemacmenu.override

igemacmenu_la_CFLAGS = $(MENU_CFLAGS) $(PYGTK_CFLAGS)
igemacmenu_la_LDFLAGS = -module -avoid-version
igemacmenu_la_LIBADD = $(MENU_LIBS) $(PYGTK_LIBS) $(top_builddir)/src/libigemacmenu.la
igemacmenu_la_SOURCES = 		\
	igemacmenu.c			\
	igemacmenu-module.c

.defs.c: 
	($(PYGTK_CODEGEN)		\
	    -I $(top_srcdir)/src	\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --override $*.override	\
	    --prefix $* $<) > gen-$*.c	\
	&& cp gen-$*.c $*.c		\
	&& rm -f gen-$*.c

CLEANFILES = igemacmenu.defs igemacmenu.c

EXTRA_DIST = igemacmenu.defs igemacmenu.overrides
