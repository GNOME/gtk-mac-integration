INCLUDES = -I$(top_srcdir)/src $(PYTHON_INCLUDES)

igemacintegrationdir = $(libdir)/python$(PYTHON_VERSION)/site-packages/igemacintegration

igemacintegration_PYTHON = __init__.py
igemacintegration_LTLIBRARIES = _igemacintegration.la

headers =						\
	$(top_srcdir)/src/ige-mac-menu.h		\
	$(top_srcdir)/src/ige-mac-dock.h		\
	$(top_srcdir)/src/ige-mac-bundle.h

igemacintegration.defs: $(headers)
	$(PYTHON) $(datadir)/pygobject/2.0/codegen/h2def.py $(headers) > $@

igemacintegration.c: igemacintegration.defs igemacintegration.override

_igemacintegration_la_CFLAGS =				\
	$(MAC_CFLAGS)					\
	$(PYGTK_CFLAGS)
_igemacintegration_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_igemacintegration
_igemacintegration_la_LIBADD =				\
	$(MAC_LIBS)					\
	$(PYGTK_LIBS)					\
	$(top_builddir)/src/libigemacintegration.la
_igemacintegration_la_SOURCES =				\
	igemacintegration.c				\
	igemacintegration-module.c

.defs.c: 
	($(PYGTK_CODEGEN)				\
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs	\
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs	\
	    --override $*.override			\
	    --prefix $* $<) > gen-$*.c			\
	&& cp gen-$*.c $*.c				\
	&& rm -f gen-$*.c

CLEANFILES =						\
	igemacintegration.defs				\
	igemacintegration.c

EXTRA_DIST =						\
	igemacintegration.defs				\
	igemacintegration.override
