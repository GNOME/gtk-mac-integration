AC_PREREQ(2.52)

AC_INIT(gtk-mac-integration, 2.0.8)
# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A

GTK_MAC_INTEGRATION_LT_VERSION=2:8:0
AC_SUBST(GTK_MAC_INTEGRATION_LT_VERSION)
AC_CONFIG_SRCDIR(src/gtk-mac-menu.c)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE([1.11 dist-bzip2 subdir-objects no-define foreign])

AC_CONFIG_HEADERS(config.h)

AM_DISABLE_STATIC
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
AC_HEADER_STDC
AC_PROG_OBJC
AM_PROG_CC_C_O
AM_PROG_AR
AM_PROG_LIBTOOL

AM_PATH_GLIB_2_0([2.14.0])
AC_MSG_CHECKING([GLib Version])
PKG_INSTALLDIR()

GTK_DOC_CHECK([1.11], [--flavour no-tmpl])

AC_ARG_ENABLE([menu-images],
    [AS_HELP_STRING([--enable-menu-images],
     [Include menu item icons in Mac menus @<:@default=no@:>@])],
    [],
    [enable_menu_images=no])
AS_IF([test "x$enable_menu_images" != xno],
      [AC_DEFINE([USE_MENU_IMAGES], 1, [Add images to menu items])])


AC_ARG_WITH([gtk2], [AS_HELP_STRING([--with-gtk2], [build against gtk2])] ,[with_gtk2=$withval],
	[with_gtk2=yes])
AC_ARG_WITH([gtk3], [AS_HELP_STRING([--with-gtk3], [build against gtk3])] ,[with_gtk3=$withval],
    [with_gtk3=yes])

if test "x$with_gtk3" = "xyes" ; then
    PKG_CHECK_MODULES([GTK3], [gtk+-3.0], [],
       with_gtk3=no
       AC_MSG_WARN([GTK+-3.0 not found])
    )
fi

if test "x$with_gtk2" = "xyes" ; then
    PKG_CHECK_MODULES([GTK2], [gtk+-2.0], [],
       with_gtk2=no
       AC_MSG_WARN([GTK+-2.0 not found])
    )
fi

if test "x$with_gtk2" = "xno" && test "x$with_gtk3" = "xno" ; then
	AC_MSG_ERROR([Neither GTK+-2.0 nor GTK+-3.0 was found])
fi

AM_CONDITIONAL([INTROSPECTION], [test x$with_gtk3 = xyes])
AM_CONDITIONAL([WITH_GTK2], [test x$with_gtk2 = xyes])
AM_CONDITIONAL([WITH_GTK3], [test x$with_gtk3 = xyes])

GTK_PYTHON_CHECK
GOBJECT_INTROSPECTION_CHECK([0.10.1])

# This will cause the automake generated makefiles to pass the correct
# flags to aclocal.
ACLOCAL_AMFLAGS="\${ACLOCAL_FLAGS}"
AC_SUBST(ACLOCAL_AMFLAGS)
if test "x$prefix" = xNONE; then
  AC_DEFINE_UNQUOTED([PREFIX],["$ac_default_prefix"], [Installation prefix (for making filenames)])
else
  AC_DEFINE_UNQUOTED([PREFIX],["$prefix"], [Installation prefix (for making filenames)])
fi

AC_CONFIG_FILES(
	Makefile
	src/Makefile
        bindings/Makefile
        bindings/python/Makefile
	bindings/python/gtkmacintegration/Makefile
	bindings/python/gtkosx_application/Makefile
        data/Makefile
	docs/Makefile
	docs/reference/Makefile
	strings/Makefile
    data/gtk-mac-integration-gtk2.pc
    data/gtk-mac-integration-gtk3.pc
)

AC_OUTPUT

echo
echo "Prefix         : $prefix"
echo "GTK+2 bindings : $with_gtk2"
echo "GTK+3 bindings : $with_gtk3"
echo "Python bindings: $enable_python"
echo "Introspection  : $enable_introspection"
echo
