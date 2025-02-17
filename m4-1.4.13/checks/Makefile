## Makefile.in - template for building Makefile for M4 testsuite.
##
## Copyright (C) 1992, 1993, 1994, 2006, 2007, 2008 Free Software
## Foundation, Inc.
##
## This file is part of GNU M4.
##
## GNU M4 is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## GNU M4 is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.


PACKAGE = m4
VERSION = 1.4.13

SHELL = /bin/bash

bindir = ${exec_prefix}/bin
exec_prefix = ${prefix}
prefix = /usr/local
srcdir = .

PATH_SEPARATOR = :
program_transform_name = s,x,x,

# Should be GNU awk, for the get-them script.
AWK = gawk

# Vern says that the first star is required around an Alpha make bug.
DOC_CHECKS = $(srcdir)/*[0-9][0-9][0-9].*
CHECKS = $(DOC_CHECKS) $(srcdir)/stackovf.test
# Makefile.in is automatically distributed by automake.
DISTFILES = $(srcdir)/get-them $(srcdir)/check-them $(srcdir)/stamp-checks \
	$(srcdir)/stackovf.test

all: $(srcdir)/stamp-checks

$(srcdir)/stamp-checks: $(srcdir)/get-them $(srcdir)/../doc/m4.texinfo
	rm -f $(DOC_CHECKS)
	cd $(srcdir) && AWK=$(AWK) ./get-them ../doc/m4.texinfo
	touch $(srcdir)/stamp-checks

install:

dvi pdf ps html info:
install-dvi install-pdf install-ps install-html install-info:

uninstall:

check: $(srcdir)/stamp-checks
	PATH=`pwd`/../src"$(PATH_SEPARATOR)"$$PATH; export PATH; \
	$(srcdir)/check-them -I $(srcdir)/../examples $(CHECKS)

installcheck: $(srcdir)/stamp-checks
	PATH='$(bindir)'"$(PATH_SEPARATOR)"$$PATH; export PATH; \
	$(srcdir)/check-them -I $(srcdir)/../examples \
	-m "`echo m4 | sed '$(program_transform_name)'`" $(CHECKS)

tags:

mostlyclean:

clean: mostlyclean

distclean: clean
	rm -f Makefile

maintainer-clean realclean: distclean
	rm -f $(DOC_CHECKS) $(srcdir)/stamp-checks

distdir: dist

dist: $(DISTFILES)
	@echo "Copying distribution files"
	@for file in $(DISTFILES) $(DOC_CHECKS); do \
	  ln $$file ../$(PACKAGE)-$(VERSION)/checks 2> /dev/null \
	    || cp -p $$file ../$(PACKAGE)-$(VERSION)/checks; \
	done

Makefile: $(srcdir)/Makefile.in ../config.status
	cd .. && ./config.status checks/$@

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
