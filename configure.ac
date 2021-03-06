#
# Copyright (C) 2014-2016 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

AC_INIT([r3], [0.0])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CC_STDC
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])
LT_INIT

# NOTE: We turn on -Wall and -Wextra immediately so as to ensure as many code
# defects as possible can be caught early on. With developer mode, such defects
# are considered errors in order to ensure the codebase is only advanced with
# "pristine" patches. We leave -Wall and -Wextra here for non-developer-mode
# builds too so that package maintainers are aware of any potential issues that
# arise.
proj_cflags="-Wall -Wextra"

dnl options which probably ought to be turned on if you're doing development
AC_MSG_CHECKING([whether or not to enable developer mode])
AC_ARG_ENABLE([developer-mode],
              AS_HELP_STRING([--enable-developer-mode],
                             [turn on common options for development]),
              [COMMONDEVOPTS="$enableval"], [])
if test "x$COMMONDEVOPTS" == "xyes"; then
    AC_MSG_RESULT([yes])
    ASSERTIONS="yes"
    BUILD_TESTS="yes"
    GCOV_SUPPORT="yes"
    proj_cflags="$proj_cflags -Werror -O0"
else
    AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([whether or not to turn on assertions])
AC_ARG_ENABLE([assertions],
              AS_HELP_STRING([--enable-assertions],
                             [turn on assert() for development]),
              [ASSERTIONS="$enableval"], [])
if test "x$ASSERTIONS" != "xyes"; then
    AC_MSG_RESULT([no])
    AC_DEFINE([NDEBUG], , [if defined, assertions do nothing])
else
    AC_MSG_RESULT([yes])
fi

AC_MSG_CHECKING([whether or not to build tests])
AC_ARG_ENABLE([build-tests],
              AS_HELP_STRING([--enable-build-tests],
                             [build the 'make check' functionality]),
              [BUILD_TESTS="$enableval"], [])
if test "x$BUILD_TESTS" == "xyes"; then
    AC_MSG_RESULT([yes])

    dnl verify that check is installed on the system.
    dnl If found, continue; if not found, error.
    PKG_CHECK_MODULES([CHECK_EXISTS], [check],
                      [], [AC_MSG_ERROR([check is not installed.])])

    dnl Tests require basic libcheck functionality, but we also want to support
    dnl libcheck TAP, if it's available. TAP requires libcheck 0.9.12 or
    dnl higher; so let's check for that, but not bail if it's unavailable.
    PKG_CHECK_MODULES([CHECK], [check >= 0.9.8],
                      [libcheck_support_tap="`$PKG_CONFIG \
                                             --atleast-version 0.9.12 check \
                                             && echo 1`"],
                      [libcheck_support_tap=""])
    AC_MSG_CHECKING([whether or not libcheck supports TAP])
    if test "x$libcheck_support_tap" != "x"; then
        AC_MSG_RESULT([supported])
        AC_DEFINE([CHECK_SUPPORTS_TAP], ,
                  [if defined, libcheck TAP calls will be compiled in])
    else
        AC_MSG_RESULT([not supported])
    fi
else
    AC_MSG_RESULT([no])
fi
AM_CONDITIONAL([BUILD_TESTS], [test "x$BUILD_TESTS" == "xyes"])

AC_MSG_CHECKING([whether or not to build with profiling support (gcov)])
AC_ARG_ENABLE([gcov-support],
              AS_HELP_STRING([--enable-gcov-support],
                             [compile with support for gcov]),
              [GCOV_SUPPORT="$enableval"], [])
if test "x$GCOV_SUPPORT" == "xyes"; then
    AC_MSG_RESULT([yes])
    proj_cflags="$proj_cflags -O0 -ggdb3 -fprofile-arcs -ftest-coverage"
else
    AC_MSG_RESULT([no])
fi

dnl currently "hard coded" configuration options
AC_DEFINE([NUM_SIDES], 6,
          [number of faces on an r3cube; currently only supports hard-coded 6])
AC_DEFINE([NUM_SIDES_RING], (NUM_SIDES - 2),
          [number of faces on an r3cube "ring"; property of the topology])
AC_DEFINE([NUM_COLS], 3,
          [number of columns on a r3side; currently only supports hard-coded 3])
AC_DEFINE([NUM_ROWS], 3,
          [number of rows on a r3side; currently only supports hard-coded 3])
AC_DEFINE([MAX_ROW_COLS], (NUM_COLS > NUM_ROWS ? NUM_COLS : NUM_ROWS),
          [the number of rows or number of columns, which ever is greater])
AC_DEFINE([MAX_NUM_BROTHERS], 2,
          [max number of possible brothers; currently only supports hard-coded
           2 (in the corner-edge case)])
AC_DEFINE([MAX_NUM_NEIGHBORS], 4,
          [max number of possible neighbors; currently only supports hard-coded
           4 (in the center-of-face case)])

AC_SUBST([PROJ_CFLAGS], [$proj_cflags])

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 lib/libr3/Makefile
                 lib/libr3/include/Makefile
                 lib/libr3/src/Makefile
                 src/Makefile
                 test/Makefile])

AC_OUTPUT

# vim: set et ts=4 sw=4 :
