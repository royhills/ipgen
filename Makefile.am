# $Id$
# Process this file with automake to produce Makefile.in
#
AM_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"
#
bin_PROGRAMS = ipgen
#
ipgen_SOURCES = ipgen.c ipgen.h error.c wrappers.c utils.c
ipgen_LDADD = $(LIBOBJS)
