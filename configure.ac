# SPDX-License-Identifier: LGPL-2.1-only

AC_INIT([librtpi], [1.0.0], [https://gitlab.com/linux-rt/librtpi/-/issues])

MAJOR=1
MINOR=0
REVISION=0
AC_SUBST([LIBRTPI_VERSION], [$MAJOR:$MINOR:$REVISION], [library version])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_ARG_ENABLE([cancellation],
    AS_HELP_STRING([--disable-cancellation], [disable cancellation support]))

AM_CONDITIONAL([ENABLE_CANCELLATION], [test "x$enable_cancellation" != "xno"])

AM_PROG_AR
LT_INIT
AC_PROG_CC
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
 tests/glibc-tests/Makefile
 tests/libstdc++-tests/Makefile
])
AC_CONFIG_MACRO_DIRS([m4])
AX_CXX_COMPILE_STDCXX_11
AC_OUTPUT
