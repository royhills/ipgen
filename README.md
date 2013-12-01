# ipgen

[![Build Status](https://secure.travis-ci.org/royhills/ipgen.png)](http://travis-ci.org/royhills/ipgen)
[![Coverage Status](https://coveralls.io/repos/royhills/ipgen/badge.png?branch=master)](https://coveralls.io/r/royhills/ipgen?branch=master)

Output a list of host address from IP network specifications.

Installation
------------

ipgen uses the standard GNU automake and autoconf tools, so the typical installation process is:

- Run ```git clone https://github.com/royhills/ipgen.git``` to obtain the project source code
- Run ```cd ipgen``` to enter source directory
- Run ```autoreconf --install``` to generate a viable ./configure file
- Run ```./configure``` to generate a makefile for your system
- Run ```make``` to build the project
- Optionally run ```make check``` to verify that everything works as expected
- Run ```make install``` to install (you'll need root or sudo for this part)

You will need GNU automake and autoconf, the make utility, an ANSI C compiler (for example gcc or clang), the development header files and libraries.

You can pass various options to "configure" to control the build and
installation process.  See the file INSTALL for more details.

ipgen is known to compile and run on the following platforms:

 - Linux (tested on Debian Etch)
 - FreeBSD (tested on FreeBSD 9.0)

Documentation
-------------

For usage information, including details of all the options, use:

```ipgen --help```

or see the man page ipgen(1).
