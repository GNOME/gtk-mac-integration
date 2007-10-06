INCLUDES = -I$(top_srcdir)/../src $(PYTHON_INCLUDES)

igemacintegrationdir = $(libdir)/python$(PYTHON_VERSION)/site-packages

igemacintegration_LTLIBRARIES = igemacintegration.la

headers =				\
	$(top_srcdir)/src/ige-mac-menu.h\
	$(top_srcdir)/src/ige-mac-dock.h

igemacintegration.defs: $(headers)
	$(PYTHON) $(datadir)/pygtk/2.0/codegen/h2def.py $< > $@

igemacintegration.c: igemacintegration.defs igemacintegration.override

igemacintegration_la_CFLAGS = $(MAC_CFLAGS) $(PYGTK_CFLAGS)
igemacintegration_la_LDFLAGS = -module -avoid-version
igemacintegration_la_LIBADD = $(MAC_LIBS) $(PYGTK_LIBS) $(top_builddir)/src/libigemacintegration.la
igemacintegration_la_SOURCES = 		\
	igemacintegration.c			\
	igemacintegration-module.c

.defs.c: 
	($(PYGTK_CODEGEN)		\
	    -I $(top_srcdir)/src	\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --override $*.override	\
	    --prefix $* $<) > gen-$*.c	\
	&& cp gen-$*.c $*.c		\
	&& rm -f gen-$*.c

CLEANFILES = igemacintegration.defs igemacintegration.c

EXTRA_DIST = igemacintegration.defs igemacintegration.overrides
