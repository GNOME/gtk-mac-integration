AC_PREREQ(2.52)

AC_INIT(gtk-mac-integration, 2.0.4)
# Before making a release, the LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A

GTK_MAC_INTEGRATION_LT_VERSION=2:4:0
AC_SUBST(GTK_MAC_INTEGRATION_LT_VERSION)
AC_CONFIG_SRCDIR(src/gtk-mac-menu.c)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS(config.h)

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
AC_HEADER_STDC
AC_PROG_OBJC
AM_PROG_CC_C_O

AM_PATH_GLIB_2_0([2.14.0])
AC_MSG_CHECKING([for GLib >= 2.31.0])
if $PKG_CONFIG 'glib-2.0 >= 2.32.0'
then
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_GLIB_2_31,1,[System has GLib 2.31.0 or newer])
fi

GTK_DOC_CHECK([1.11], [--flavour no-tmpl])

AC_ARG_ENABLE([menu-images],
    [AS_HELP_STRING([--enable-menu-images],
     [Include menu item icons in Mac menus @<:@default=no@:>@])],
    [],
    [enable_menu_images=no])
AS_IF([test "x$enable_menu_images" != xno],
      [AC_DEFINE([USE_MENU_IMAGES], 1, [Add images to menu items])])

AC_ARG_WITH([gtk],
	[AS_HELP_STRING([--with-gtk],
		[select gtk+-3.0 or gtk+-2.0. @<:@default=check@:>@])],
	[],
	[with_gtk=check])
dnl gtk_major=

AS_ECHO(["Specified Gtk Version $with_gtk"])

AS_IF([test "x$with_gtk" = xgtk+-3.0],
         [PKG_CHECK_MODULES(MAC, [gtk+-3.0 >= 2.90], GTK_MAJOR="gtk+-3.0",
           AC_MSG_ERROR([GTK+-3.0 specified but not found]))],
      [test "x$with_gtk" = xgtk+-2.0],
         [PKG_CHECK_MODULES(MAC, [gtk+-2.0 >= 2.10], GTK_MAJOR="gtk+-2.0",
           AC_MSG_ERROR([GTK+-2.0 specified but not found]))],
      [PKG_CHECK_MODULES(MAC, [gtk+-3.0 >= 2.90],GTK_MAJOR="gtk+-3.0",
         PKG_CHECK_MODULES(MAC, [gtk+-2.0 >= 2.10], GTK_MAJOR="gtk+-2.0",
	    AC_MSG_ERROR([GTK+-2.0 Wersion 2.10 or higher or GTK+-3.0 Version 2.90.0 or higher is required to build gtk-mac-integration])))])

AC_MSG_CHECKING([GTK+ Version])

GTK_VERSION=`$PKG_CONFIG --modversion ${GTK_MAJOR}`
AC_SUBST(GTK_MAJOR)
AC_SUBST(GTK_VERSION)
AC_MSG_RESULT("$GTK_VERSION")
AM_CONDITIONAL([INTROSPECTION], [test x$GTK_MAJOR = "xgtk+-3.0"])

AS_IF([test "$GTK_MAJOR" = gtk+-3.0],
	 [PKG_CHECK_EXISTS([gtk+-3.0 >= 3.3.0],
	 AC_DEFINE([HAVE_GTK_34], 1, [Gtk version 3.3 or higher]))
	 PKG_CHECK_EXISTS([gtk+-3.0 >= 3.9.0],
	 AC_DEFINE([HAVE_GTK_310], 1, [Gtk version 3.9 or higher]))],)

GTK_PYTHON_CHECK

GOBJECT_INTROSPECTION_CHECK([0.10.1])

# This will cause the automake generated makefiles to pass the correct
# flags to aclocal.
ACLOCAL_AMFLAGS="\${ACLOCAL_FLAGS}"
AC_SUBST(ACLOCAL_AMFLAGS)
AC_DEFINE_UNQUOTED([PREFIX],["$prefix"], [Installation prefix (for making filenames)])

AC_CONFIG_FILES(
	Makefile
	src/Makefile
        bindings/Makefile
        bindings/python/Makefile
	bindings/python/gtkmacintegration/Makefile
	bindings/python/gtkosx_application/Makefile
        data/Makefile
        data/gtk-mac-integration.pc
	docs/Makefile
	docs/reference/Makefile
	strings/Makefile
)

AC_OUTPUT

echo
echo "Prefix         : $prefix"
echo "Python bindings: $enable_python"
echo
