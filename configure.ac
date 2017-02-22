AC_PREREQ([2.68])
AC_INIT([cxi], [m4_esyscmd([build-aux/git-version-gen .tarball-version])], [clamberson@gmail.com], [], [https://github.com/dutch/cxi/issues], [https://github.com/dutch/cxi])
AC_CONFIG_SRCDIR([src/cxi.c])
AC_CONFIG_AUX_DIR([build-aux])
AX_CHECK_HOST
AM_INIT_AUTOMAKE([1.14 foreign tar-ustar dist-xz no-dist-gzip subdir-objects -Wall -Werror])
AM_SILENT_RULES([yes])
AC_PROG_CC
PKG_CHECK_MODULES([CURSES], [ncurses])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
