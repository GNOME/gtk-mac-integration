INCLUDES = -I$(top_srcdir)/src $(PYTHON_INCLUDES)

gtk_osxapplicationdir = $(libdir)/python$(PYTHON_VERSION)/site-packages/gtk_osxapplication

gtk_osxapplication_PYTHON = __init__.py
gtk_osxapplication_LTLIBRARIES = _gtk_osxapplication.la

headers = $(top_srcdir)/src/gtkosxapplication.h

gtk_osxapplication.defs: $(headers)
	$(PYTHON) $(datadir)/pygobject/2.0/codegen/h2def.py $(headers) > $@

gtk_osxapplication.c: gtk_osxapplication.defs gtk_osxapplication.override

_gtk_osxapplication_la_CFLAGS =				\
	$(MAC_CFLAGS)					\
	$(PYGTK_CFLAGS)
_gtk_osxapplication_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_gtk_osxapplication
_gtk_osxapplication_la_LIBADD =				\
	$(MAC_LIBS)					\
	$(PYGTK_LIBS)					\
	$(top_builddir)/src/libgtkmacintegration.la
_gtk_osxapplication_la_SOURCES =				\
	gtk_osxapplication.c				\
	gtk_osxapplication-module.c

.defs.c: 
	($(PYGOBJECT_CODEGEN)				\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs	\
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs	\
	    --override $*.override			\
	    --prefix $* $<) > gen-$*.c			\
	&& cp gen-$*.c $*.c				\
	&& rm -f gen-$*.c

CLEANFILES =						\
	gtk_osxapplication.defs				\
	gtk_osxapplication.c

EXTRA_DIST =						\
	gtk_osxapplication.defs				\
	gtk_osxapplication.override
