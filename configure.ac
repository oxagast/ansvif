AC_INIT([ansvif], [1.3.5], [marshallwhittaker@gmail.com])
AM_INIT_AUTOMAKE

AC_CANONICAL_HOST
case "$host" in
     *-cygwin*)
     if test "$with_cygwin" != 'no'; then
       AC_DEFINE(HAVE_CYGWIN,,Define if compiling under cygwin)
       have_cygwin='yes'
       separator="\\\\\\\\"
       mv include/gzstream/gzstream.h include/gzstream/gzstream-linux.h
       mv include/gzstream/gzstream-win.h include/gzstream/gzstream.h
       
     fi
     ;;
     *-apple-darwin*)
     have_aqua='yes'
     ;;
     i686-pc-linux-gnu)
     ;;
esac


AM_CONDITIONAL(HAVE_CYGWIN, test "$have_cygwin" = "yes")
AM_CONDITIONAL(USE_AR, test $use_ar = 'yes')
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
AC_CONFIG_FILES([Makefile])
AM_INIT_AUTOMAKE([subdir-objects])
AC_OUTPUT
