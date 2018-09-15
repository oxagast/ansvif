AC_INIT([ansvif], [1.10], [marshallwhittaker@gmail.com], [], [https://oxagast.github.io/ansvif])
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++])
AC_TYPE_SIZE_T
AC_PROG_RANLIB
#AC_CONFIG_HEADERS([include/gzstream/gzstream.h])
AC_HEADER_STDC
#AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
AX_CXX_COMPILE_STDCXX_11
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([ipstream])
AC_CHECK_HEADERS([lz])
AC_CHECK_HEADERS([crypto])
AC_CONFIG_FILES([Makefile])
AM_INIT_AUTOMAKE([subdir-objects])
# This adds the option of compiling without using the ctemplate library,
# which has proved troublesome for compilation on some platforms
AC_ARG_ENABLE(syscalls,
[  --enable-syscalls       Enable Syscalls compiliation (experimental)],
[case "${enableval}" in
  yes) WITH_SYSCALLS=true ;;
  no)  WITH_SYSCALLS=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-syscalls) ;;
esac],[WITH_SYSCALLS=false])
AM_CONDITIONAL([WITH_SYSCALLS], [test "$WITH_SYSCALLS" = "true"])
# try to compile syscalls if they're enabled
if test "$WITH_SYSCALLS" = "true"; then
    AC_DEFINE([WITH_SYSCALLS], [], ["build syscalls module"])
    AC_MSG_NOTICE([syscalls enabled, we will try to compile them])
else
    AC_MSG_NOTICE([syscalls module is disabled])
fi

AC_ARG_ENABLE(gtk,
[  --disable-gtk       Enable GTK compiliation (experimental)],
[case "${enableval}" in
  yes) WITH_GTK=true ;;
  no)  WITH_GTK=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-gtk) ;;
esac],[WITH_GTK=true])
AM_CONDITIONAL([WITH_GTK], [test "$WITH_GTK" = "true"])
# try to compile syscalls if they're enabled
if test "$WITH_GTK" = "false"; then
    AC_MSG_NOTICE([GTK module is disabled])
else
    AC_DEFINE([WITH_GTK], [], ["build GTK module"])
    AC_MSG_NOTICE([GTK enabled, we will try to compile them])
    PKG_CHECK_MODULES([GTK], [gtk+-2.0])
#     PKG_CHECK_MODULES([gtk+-2.0])
fi

AC_ARG_ENABLE(testcode,
	      [  --disable-testcode       Disable Test Code)],
	      [case "${enableval}" in
	       yes) WITH_TESTCODE=true ;;
	       no)  WITH_TESTCODE=false ;;
	     *) AC_MSG_ERROR(bad value ${enableval} for --disable-testcode) ;;
	     esac],[WITH_GTK=true])
AM_CONDITIONAL([WITH_TESTCODE], [test "$WITH_TESTCODE" = "true"])
if test "$WITH_TESTCODE" = "false"; then
    AC_MSG_NOTICE([Test Code is disabled])
else
AC_DEFINE([WITH_TESTCODE], [], ["build Test Code"])
AC_MSG_NOTICE([Test Code, we will try to compile them])
fi


AM_CONDITIONAL(OS_IS_REDHAT, [ test `grep "Fedora\|Redhat" /etc/os-release | wc -l` -gt "0" ])
if test "$OS_IS_REDHAT" = "0"; then
    AC_DEFINE([__REDHAT], [], ["build for RedHat"])
    AC_MSG_NOTICE([trying to build for a RedHat based system])
fi
AM_CONDITIONAL(OS_IS_DEBIAN, [ test `grep "Debian\|Ubuntu\|Kali" /etc/os-release | wc -l` -gt "0" ])
if test "$OS_IS_DEBIAN" = "0"; then
    AC_DEFINE([__DEBIAN], [], ["build for Debian"])
    AC_MSG_NOTICE([trying to build for a Debian based system])
fi
AM_CONDITIONAL(OS_IS_KALI, [ test `grep "Kali" /etc/os-release | wc -l` -gt "0" ])
if test "$OS_IS_KALI" = "0"; then
   AC_DEFINE([__KALI__], [], ["build for Kali"])
   AC_MSG_NOTICE([trying to build for a Kali based system])
fi
AM_CONDITIONAL(OS_IS_FREEBSD, [ test `uname -o` == "FreeBSD" ])
if test "$IS_IS_FREEBSD" = "0"; then
    AC_DEFINE([__UNKNOWN], [], ["build for FreeBSD"])
    AC_MSG_NOTICE([trying to build for a FreeBSD based system])
fi
AM_CONDITIONAL(OS_IS_UNKNOWN, [ test `test -e /dummy; echo $?` -eq "0" ])
if test "$OS_IS_UNKNOWN" = "0"; then
    AC_DEFINE([__UNKNOWN], [], ["build for Unknown"])
    AC_MSG_NOTICE([trying to build for a unknown system])
fi
AC_OUTPUT
