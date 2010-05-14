AC_PREREQ(2.52)
 
AC_INIT(ige-mac-integration, 0.8.6)
AC_CONFIG_SRCDIR(src/ige-mac-menu.c)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AM_CONFIG_HEADER(config.h)

AM_DISABLE_STATIC
AM_PROG_LIBTOOL
AC_MSG_CHECKING([Bitness])
AC_PREPROC_IFELSE(
    [AC_LANG_PROGRAM([[]],[[#ifndef __x86_64__
#error
#endif
	]])],
    [AC_MSG_RESULT([64-BIT])
     sixty_four_bit=yes],
    [AC_MSG_RESULT([32-Bit])
     sixty_four_bit=no])
AM_CONDITIONAL([SIXTY_FOUR_BIT], [test x$sixty_four_bit = xyes])
AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PROG_OBJC

AM_PATH_GLIB_2_0

GTK_DOC_CHECK([1.11], [--flavour no-tmpl])

IMENDIO_COMPILE_WARNINGS
IMENDIO_PYTHON_CHECK

PKG_CHECK_MODULES(MAC, [gtk+-2.0 >= 2.13])

# This will cause the automake generated makefiles to pass the correct
# flags to aclocal.
ACLOCAL_AMFLAGS="\${ACLOCAL_FLAGS}"
AC_SUBST(ACLOCAL_AMFLAGS)
AC_DEFINE_UNQUOTED([PREFIX],["$prefix"], [Installation prefix (for making filenames)])

AC_OUTPUT([
	Makefile 
	src/Makefile
        bindings/Makefile
        bindings/python/Makefile
	bindings/python/igemacintegration/Makefile
	bindings/python/gtk_osxapplication/Makefile
        data/Makefile
        data/ige-mac-integration.pc
])

echo
echo "Prefix         : $prefix"
echo "Python bindings: $enable_python"
echo
