AC_DEFUN([AX_CHECK_HOST],
[AC_REQUIRE([AC_CANONICAL_HOST])
build_linux=no
build_mac=no
case "${host_os}" in
    linux*)
        build_linux=yes
        ;;
    darwin*)
        build_mac=yes
        ;;
    *)
        AC_MSG_ERROR([OS $host_os is not supported])
        ;;
esac
AM_CONDITIONAL([LINUX], [test x$build_linux = xyes])
AM_CONDITIONAL([MAC], [test x$build_mac = xyes])
])
