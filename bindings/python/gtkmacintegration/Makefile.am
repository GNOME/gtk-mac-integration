AM_CPPFLAGS = -I$(top_srcdir)/src $(PYTHON_INCLUDES)

gtkmacintegrationdir = $(libdir)/python$(PYTHON_VERSION)/site-packages/gtkmacintegration

gtkmacintegration_PYTHON = __init__.py
gtkmacintegration_LTLIBRARIES =



headers =						\
	$(top_srcdir)/src/gtk-mac-menu.h		\
	$(top_srcdir)/src/gtk-mac-dock.h		\
	$(top_srcdir)/src/gtk-mac-bundle.h

gtkmacintegration.defs: $(headers)
	$(PYTHON) $(datadir)/pygobject/2.0/codegen/h2def.py $(headers) > $@

gtkmacintegration.c: gtkmacintegration.defs gtkmacintegration.override

if WITH_GTK2
gtkmacintegration_LTLIBRARIES += _gtkmacintegration-gtk2.la
_gtkmacintegration_gtk2_la_CFLAGS =				\
	$(MAC_CFLAGS)					\
	$(PYGTK_CFLAGS)
_gtkmacintegration_gtk2_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_gtkmacintegration
_gtkmacintegration_gtk2_la_LIBADD =				\
	$(MAC_LIBS)					\
	$(PYGTK_LIBS)					\
	$(top_builddir)/src/libgtkmacintegration-gtk2.la

_gtkmacintegration_gtk2_la_SOURCES =				\
	gtkmacintegration.c				\
	gtkmacintegration-module.c
endif
if WITH_GTK3
gtkmacintegration_LTLIBRARIES += _gtkmacintegration-gtk3.la
_gtkmacintegration_gtk3_la_CFLAGS =				\
	$(MAC_CFLAGS)					\
	$(PYGTK_CFLAGS)
_gtkmacintegration_gtk3_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_gtkmacintegration
_gtkmacintegration_gtk3_la_LIBADD =				\
	$(MAC_LIBS)					\
	$(PYGTK_LIBS)					\
	$(top_builddir)/src/libgtkmacintegration-gtk3.la

_gtkmacintegration_gtk3_la_SOURCES =				\
	gtkmacintegration.c				\
	gtkmacintegration-module.c
endif

.defs.c: 
	($(PYGOBJECT_CODEGEN)				\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs	\
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs	\
	    --override ${srcdir}/$*.override			\
	    --prefix $* $<) > gen-$*.c			\
	&& cp gen-$*.c $*.c				\
	&& rm -f gen-$*.c

CLEANFILES =						\
	gtkmacintegration.defs				\
	gtkmacintegration.c

EXTRA_DIST =						\
	gtkmacintegration.defs				\
	gtkmacintegration.override
