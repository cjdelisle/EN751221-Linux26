# ../gui/Makefile.  Generated from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for GUI for Roaring Penguin's Linux user-space PPPoE client.
#
# Copyright (C) 2001 Roaring Penguin Software Inc.
#
# This program may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
#
# LIC: GPL
#
# $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/rp-pppoe-3.10/gui/Makefile#1 $
#***********************************************************************
DEFINES=
prefix=/usr
exec_prefix=${prefix}
mandir=${prefix}/share/man
install=/usr/bin/install -c
install_dir=/usr/bin/install -c -d
sbindir=${exec_prefix}/sbin
bindir=${exec_prefix}/bin
datadir=${prefix}/share

PPPOE_START_PATH=${exec_prefix}/sbin/pppoe-start
PPPOE_STOP_PATH=${exec_prefix}/sbin/pppoe-stop
PPPOE_STATUS_PATH=${exec_prefix}/sbin/pppoe-status

PATHS='-DPPPOE_START_PATH="$(PPPOE_START_PATH)"' '-DPPPOE_STOP_PATH="$(PPPOE_STOP_PATH)"' '-DPPPOE_STATUS_PATH="$(PPPOE_STATUS_PATH)"'

CFLAGS= -g -O2 -Wall -Wstrict-prototypes $(DEFINES) $(PATHS)

all: pppoe-wrapper
	@echo ""
	@echo "Type 'make install' as root to install the software."

pppoe-wrapper: wrapper.o
	gcc -o pppoe-wrapper wrapper.o

wrapper.o: wrapper.c
	gcc $(CFLAGS) -c -o wrapper.o wrapper.c

install: all
	-mkdir -p $(DESTDIR)$(sbindir)
	-mkdir -p $(DESTDIR)$(bindir)
	-mkdir -p $(DESTDIR)/etc/ppp/rp-pppoe-gui
	$(install) -m 4755 -s pppoe-wrapper $(DESTDIR)$(sbindir)
	$(install) -m 755 tkpppoe $(DESTDIR)$(bindir)
	-mkdir -p $(DESTDIR)$(mandir)/man1
	$(install) -m 644 pppoe-wrapper.1 $(DESTDIR)$(mandir)/man1
	$(install) -m 644 tkpppoe.1 $(DESTDIR)$(mandir)/man1
	-mkdir -p $(DESTDIR)$(datadir)/tkpppoe
	for i in tkpppoe.html mainwin-busy.png mainwin-nonroot.png mainwin.png props-advanced.png props-basic.png props-nic.png props-options.png; do \
		$(install) -m 644 html/$$i $(DESTDIR)$(datadir)/tkpppoe; \
	done
	for i in en.msg ja.msg; do \
		$(install) -m 644 $$i $(DESTDIR)$(datadir)/tkpppoe; \
	done

clean:
	rm -f *.o *~ pppoe-wrapper

distclean: clean
	rm -f Makefile tkpppoe

.PHONY: clean

.PHONY: distclean
