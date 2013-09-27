AM_CPPFLAGS = -I$(top_srcdir)/src $(PYTHON_INCLUDES)

gtkosx_applicationdir = $(libdir)/python$(PYTHON_VERSION)/site-packages/gtkosx_application

gtkosx_application_PYTHON = __init__.py
gtkosx_application_LTLIBRARIES = _gtkosx_application.la

headers = $(top_srcdir)/src/gtkosxapplication.h

gtkosx_application.defs: $(headers)
	$(PYTHON) $(datadir)/pygobject/2.0/codegen/h2def.py $(headers) > $@

gtkosx_application.c: gtkosx_application.defs

_gtkosx_application_la_CFLAGS =				\
	$(MAC_CFLAGS)					\
	$(PYGTK_CFLAGS)
_gtkosx_application_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_gtkosx_application
_gtkosx_application_la_LIBADD =				\
	$(MAC_LIBS)					\
	$(PYGTK_LIBS)					\
	$(top_builddir)/src/libgtkmacintegration.la
_gtkosx_application_la_SOURCES =				\
	gtkosx_application.c				\
	gtkosx_application-module.c

.defs.c: 
	($(PYGOBJECT_CODEGEN)				\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs	\
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs	\
	    --override $(srcdir)/$*.override			\
	    --prefix $* $<) > gen-$*.c			\
	&& cp gen-$*.c $*.c				\
	&& rm -f gen-$*.c

CLEANFILES =						\
	gtkosx_application.defs				\
	gtkosx_application.c

EXTRA_DIST =						\
	gtkosx_application.override
