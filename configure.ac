AC_PREREQ(2.52)
 
AC_INIT(ige-mac-integration, 0.8.3)
AC_CONFIG_SRCDIR(src/ige-mac-menu.c)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AM_CONFIG_HEADER(config.h)

AM_DISABLE_STATIC
AM_PROG_LIBTOOL

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

AM_PATH_GLIB_2_0

GTK_DOC_CHECK(1.7)

IMENDIO_COMPILE_WARNINGS
IMENDIO_PYTHON_CHECK

PKG_CHECK_MODULES(MAC, [gtk+-2.0 >= 2.13])

# This will cause the automake generated makefiles to pass the correct
# flags to aclocal.
ACLOCAL_AMFLAGS="\${ACLOCAL_FLAGS}"
AC_SUBST(ACLOCAL_AMFLAGS)

AC_OUTPUT([
	Makefile 
	src/Makefile
        bindings/Makefile
        bindings/python/Makefile
        data/Makefile
        data/ige-mac-integration.pc
])

echo
echo "Prefix         : $prefix"
echo "Python bindings: $enable_python"
echo
