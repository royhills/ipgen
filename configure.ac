dnl $Id$
dnl Process this file with autoconf to produce a configure script.

AC_INIT([ipgen],[1.0],[ipgen@nta-monitor.com])
AC_PREREQ(2.61)
AC_REVISION($Revision$)
AC_CONFIG_SRCDIR([ipgen.c])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

dnl Checks for programs.
AC_PROG_CC
if test -n "$GCC"; then
   AC_DEFINE([ATTRIBUTE_UNUSED], [__attribute__ ((__unused__))],
             [Define to the compiler's unused pragma])
   CFLAGS="$CFLAGS -Wall -Wshadow -Wwrite-strings"
   GCC_WEXTRA
   GCC_STACK_PROTECT_CC
   GCC_FORTIFY_SOURCE
   GCC_FORMAT_SECURITY
dnl Uncomment the line below to compile with additional warnings enabled.
dnl CFLAGS="$CFLAGS -pedantic -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs"
dnl Uncomment the line below to compile with gcov support.
dnl   CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
else
   AC_DEFINE([ATTRIBUTE_UNUSED], [],
             [Define to the compiler's unused pragma])
fi
AC_PROG_INSTALL
AC_PROG_LN_S

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h sys/socket.h sys/time.h unistd.h getopt.h pcap.h sys/ioctl.h sys/stat.h fcntl.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Check for the uint{8,16,32}_t types and, if we don't have them, define
dnl them using types which will work on most systems.
AC_NTA_CHECK_TYPE(uint8_t, unsigned char)
AC_NTA_CHECK_TYPE(uint16_t, unsigned short)
AC_NTA_CHECK_TYPE(uint32_t, unsigned int)

dnl Checks for library functions.
AC_CHECK_FUNCS([malloc inet_ntoa memset strerror])

dnl Check if the Posix regular expression functions "regcomp" and "regexec"
dnl and the header file "regex.h" are present.
AC_MSG_CHECKING([for posix regular expression support])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <regex.h>]],
[[regcomp(0, 0, 0);
regexec(0, 0, 0, 0, 0)]])],
[ac_nta_posix_regex=yes],
[ac_nta_posic_regex=no])
AC_MSG_RESULT([$ac_nta_posix_regex])
if test $ac_nta_posix_regex = no; then
   AC_MSG_ERROR([You don't seem to have posix regular expression support])
else
   AC_DEFINE(HAVE_REGEX_H, 1, [Define to 1 if you have posix regex support])
fi

dnl GNU systems e.g. Linux have getopt_long_only, but many other systems
dnl e.g. FreeBSD 4.3 and Solaris 8 do not.  For systems that don't have it,
dnl use the GNU getopt sources (obtained from glibc).
AC_CHECK_FUNC([getopt_long_only], ,
  [ AC_LIBOBJ(getopt)
    AC_LIBOBJ(getopt1)
    AC_LIBSOURCE(getopt.h) ])

dnl Do we have inet_aton?  Most systems do, but some e.g. Solaris don't
dnl If we don't have it, then use Russ Allbery's implementation as a
dnl replacement function.
AC_CHECK_FUNC(inet_aton, , [ AC_LIBOBJ(inet_aton) ])

dnl Check for strlcat and strlcpy.  If we don't have them, use the replacement
dnl functions from OpenBSD.  Most modern C libraries have these functions,
dnl but some such as as glibc don't.
AC_CHECK_FUNC([strlcat],
              [AC_DEFINE(HAVE_STRLCAT, 1, [Define to 1 if the C library includes the strlcat function])],
              [AC_LIBOBJ(strlcat)])
AC_CHECK_FUNC([strlcpy],
              [AC_DEFINE(HAVE_STRLCPY, 1, [Define to 1 if the C library includes the strlcpy function])],
              [AC_LIBOBJ(strlcpy)])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
