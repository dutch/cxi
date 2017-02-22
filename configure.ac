AC_PREREQ([2.68])
AC_INIT([cxi], [0.1], [clamberson@gmail.com], [], [https://github.com/dutch/cxi/issues], [https://github.com/dutch/cxi])
AC_CONFIG_SRCDIR([src/cxi.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.14 foreign tar-ustar dist-xz no-dist-gzip subdir-objects -Wall -Werror])
AM_SILENT_RULES([yes])
AC_PROG_CC
PKG_CHECK_MODULES([CURSES], [ncurses])
AX_CHECK_HOST
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
