# $Id$
# Process this file with automake to produce Makefile.in
#
AM_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"
#
bin_PROGRAMS = ipgen
#
dist_check_SCRIPTS = check-run1 check-output
#
dist_man_MANS = ipgen.1
#
ipgen_SOURCES = ipgen.c ipgen.h error.c wrappers.c utils.c
ipgen_LDADD = $(LIBOBJS)
#
TESTS = $(dist_check_SCRIPTS)
