dnl -*- mode: autoconf -*-

dnl Turn on the additional warnings last, so -Werror doesn't affect other tests.
AC_DEFUN([IMENDIO_COMPILE_WARNINGS],[
   if test -f $srcdir/autogen.sh; then
        default_compile_warnings="error"
    else
        default_compile_warnings="no"
    fi
                                                                                
    AC_ARG_WITH(compile-warnings, [  --with-compile-warnings=[no/yes/error] Compiler warnings ], [enable_compile_warnings="$withval"], [enable_compile_warnings="$default_compile_warnings"])
                                                                                
    warnCFLAGS=
    if test "x$GCC" != xyes; then
        enable_compile_warnings=no
    fi
                                                                                
    warning_flags=
    realsave_CFLAGS="$CFLAGS"
                                                                                
    case "$enable_compile_warnings" in
    no)
        warning_flags=
        ;;
    yes)
        warning_flags="-Wall -Wunused -Wmissing-prototypes -Wmissing-declarations"
        ;;
    maximum|error)
        warning_flags="-Wall -Wunused -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wcast-align -std=c99"
        CFLAGS="$warning_flags $CFLAGS"
        for option in -Wno-sign-compare -Wno-pointer-sign; do
                SAVE_CFLAGS="$CFLAGS"
                CFLAGS="$CFLAGS $option"
                AC_MSG_CHECKING([whether gcc understands $option])
                AC_TRY_COMPILE([], [],
                        has_option=yes,
                        has_option=no,)
                CFLAGS="$SAVE_CFLAGS"
                AC_MSG_RESULT($has_option)
                if test $has_option = yes; then
                  warning_flags="$warning_flags $option"
                fi
                unset has_option
                unset SAVE_CFLAGS
        done
        unset option
        if test "$enable_compile_warnings" = "error" ; then
            warning_flags="$warning_flags -Werror"
        fi
        ;;
    *)
        AC_MSG_ERROR(Unknown argument '$enable_compile_warnings' to --enable-compile-warnings)
        ;;
    esac
    CFLAGS="$realsave_CFLAGS"
    AC_MSG_CHECKING(what warning flags to pass to the C compiler)
    AC_MSG_RESULT($warning_flags)
                                                                                
    WARN_CFLAGS="$warning_flags"
    AC_SUBST(WARN_CFLAGS)
])


dnl THIS IS NOT AVAILABLE ON TIGER, including a copy:
dnl a macro to check for ability to create python extensions
dnl  AM_CHECK_PYTHON_HEADERS([ACTION-IF-POSSIBLE], [ACTION-IF-NOT-POSSIBLE])
dnl function also defines PYTHON_INCLUDES
AC_DEFUN([AM_CHECK_PYTHON_HEADERS],
 [
  AC_REQUIRE([AM_PATH_PYTHON])
  AC_MSG_CHECKING(for headers required to compile python extensions)

  dnl deduce PYTHON_INCLUDES
  py_prefix=`$PYTHON -c "import sys; print sys.prefix"`
  py_exec_prefix=`$PYTHON -c "import sys; print sys.exec_prefix"`
  PYTHON_INCLUDES="-I${py_prefix}/include/python${PYTHON_VERSION}"

  if test "$py_prefix" != "$py_exec_prefix"; then
    PYTHON_INCLUDES="$PYTHON_INCLUDES -I${py_exec_prefix}/include/python${PYTHON_VERSION}"
  fi
  AC_SUBST(PYTHON_INCLUDES)

  dnl check if the headers exist:
  save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
AC_TRY_CPP([#include <Python.h>],dnl
[AC_MSG_RESULT(found)
$1],dnl
[AC_MSG_RESULT(not found)
$2])
CPPFLAGS="$save_CPPFLAGS"
])

dnl
dnl Check for python + pygtk and setup pygtk bindings variables
dnl
AC_DEFUN([IMENDIO_PYTHON_CHECK],
[
  dnl for overriding the documentation installation directory
  dnl AC_ARG_WITH([python-dir],
  dnl   AS_HELP_STRING([--with-python-dir=PATH], [path to installed Python extensions]),,
  dnl   [with_python_dir='${datadir}/python/site-packages'])
  dnl PYTHON_DIR="$with_python_dir"
  dnl AC_SUBST([PYTHON_DIR])

  AC_ARG_ENABLE([python],
    AS_HELP_STRING([--enable-python],
                   [build Python bindings [[default=yes]]]),,
    [enable_python=yes])

  if test x$enable_python = xyes; then
    dnl Check for Python
    AM_PATH_PYTHON(2.3.5,,[AC_MSG_ERROR([Python 2.3.5 or newer is required])])
    AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR([Python headers not found])])

    dnl Check for PyGTK
    PKG_CHECK_MODULES(PYGTK, pygtk-2.0 >= 2.12.0,,[AC_MSG_ERROR([PyGTK 2.12.0 or newer])])

    AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0, no)
    if test "x$PYGTK_CODEGEN" = xno; then
      AC_MSG_ERROR([pygtk-codegen-2.0 script not found])
    fi

    AC_MSG_CHECKING(for pygtk defs)
    PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
    AC_SUBST(PYGTK_DEFSDIR)
    AC_MSG_RESULT($PYGTK_DEFSDIR)
  fi

  AC_MSG_CHECKING([whether to build Python bindings])
  AC_MSG_RESULT($enable_python)

  AM_CONDITIONAL([ENABLE_PYTHON], [test x$enable_python = xyes])
])
dnl -----------------------------------------------------------
