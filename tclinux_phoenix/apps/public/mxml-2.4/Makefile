#
# "$Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/mxml-2.4/Makefile#1 $"
#
# Makefile for Mini-XML, a small XML-like file parsing library.
#
# Copyright 2003-2007 by Michael Sweet.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

#
# Compiler tools definitions...
#

AR		=	/usr/bin/ar
ARFLAGS		=	crvs
ARCHFLAGS	=	
#CC		=	mips-linux-gcc
CFLAGS		=	$(OPTIM) $(ARCHFLAGS) -Wall   
CP		=	/bin/cp
DSO		=	$(CC)
DSOFLAGS	=	 -Wl,-soname,libmxml.so.1,-rpath,$(libdir) -shared $(OPTIM)
LDFLAGS		=	$(OPTIM) $(ARCHFLAGS)  -Wl,-rpath,$(libdir)
INSTALL		=	/usr/bin/install -c
LIBMXML		=	libmxml.so.1.4
LIBS		=	 
LN		=	/bin/ln -s
MKDIR		=	/bin/mkdir
OPTIM		=	-fPIC -Os -g -DTC_SOC
RANLIB		=	$(CROSS)ranlib
RM		=	/bin/rm -f
SHELL		=	/bin/sh


#
# Configured directories...
#

prefix		=	/usr/local
exec_prefix	=	/usr/local
bindir		=	${exec_prefix}/bin
datarootdir	=	@datarootdir@
includedir	=	${prefix}/include
libdir		=	../lib
privatelibdir	=	$(LIB_DIR)/
mandir		=	${prefix}/man
docdir		=	${prefix}/share/doc/mxml
BUILDROOT	=	$(DSTROOT)


#
# Install commands...
#

INSTALL_BIN	=	$(LIBTOOL) $(INSTALL) -m 755 -s
INSTALL_DATA	=	$(INSTALL) -m 644
INSTALL_DIR	=	$(INSTALL) -d
INSTALL_LIB	=	$(LIBTOOL) $(INSTALL) -m 755
INSTALL_MAN	=	$(INSTALL) -m 644
INSTALL_SCRIPT	=	$(INSTALL) -m 755


#
# Rules...
#

.SUFFIXES:	.c .man .o
.c.o:
	$(CC) $(CFLAGS) -c $<


#
# Targets...
#

DOCFILES	=	doc/0.gif doc/1.gif doc/2.gif doc/3.gif doc/4.gif \
			doc/A.gif doc/B.gif doc/C.gif doc/D.gif \
			doc/mxml.html doc/mxmldoc.xsd \
			README COPYING CHANGES
PUBLIBOBJS	=	mxml-attr.o mxml-entity.o mxml-file.o mxml-index.o \
			mxml-node.o mxml-search.o mxml-set.o
LIBOBJS		=	$(PUBLIBOBJS) mxml-private.o mxml-string.o
OBJS		=	mxmldoc.o testmxml.o $(LIBOBJS)
#TARGETS		=	$(LIBMXML) mxmldoc testmxml mxml.xml doc/mxml.man
TARGETS		=	$(LIBMXML) mxmldoc mxml.xml

#
# Make everything...
#

all:		$(TARGETS)


#
# Clean everything...
#

clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) mxmldoc-static libmxml.a


#
# Really clean everything...
#

distclean:	clean
	#$(RM) config.cache config.log config.status
	#$(RM) Makefile config.h
	#$(RM) -r autom4te*.cache
	$(RM) *.bck *.bak


#
# Install everything...
#

install:	$(TARGETS) install-$(LIBMXML) install-libmxml.a
#	$(INSTALL_DIR) $(BUILDROOT)$(bindir)
#	$(INSTALL_BIN) mxmldoc $(BUILDROOT)$(bindir) 
#	$(INSTALL_DIR) $(BUILDROOT)$(docdir)
#	for file in $(DOCFILES); do \
#		$(INSTALL_MAN) $$file $(BUILDROOT)$(docdir); \
#	done
#	$(INSTALL_DIR) $(BUILDROOT)$(includedir)
#	$(INSTALL_DATA) mxml.h $(BUILDROOT)$(includedir)
#	$(INSTALL_DIR) $(BUILDROOT)$(libdir)/pkgconfig
#	$(INSTALL_DATA) mxml.pc $(BUILDROOT)$(libdir)/pkgconfig
#	$(INSTALL_DIR) $(BUILDROOT)$(mandir)/man1
#	$(INSTALL_MAN) doc/mxmldoc.man $(BUILDROOT)$(mandir)/man1/mxmldoc.1
#	$(INSTALL_DIR) $(BUILDROOT)$(mandir)/man3
#	$(INSTALL_MAN) doc/mxml.man $(BUILDROOT)$(mandir)/man3/mxml.3

install-libmxml.a:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libmxml.a $(BUILDROOT)$(libdir)
	$(RANLIB) $(BUILDROOT)$(libdir)/libmxml.a
	$(CP) -rf $(libdir)/libmxml.a $(privatelibdir)

install-libmxml.so.1.4:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libmxml.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so
	$(LN) libmxml.so.1.4 $(BUILDROOT)$(libdir)/libmxml.so
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so.1
	$(LN) libmxml.so.1.4 $(BUILDROOT)$(libdir)/libmxml.so.1
	
	$(CP) -rf $(libdir)/libmxml.so $(privatelibdir)
	$(CP) -rf $(libdir)/libmxml.so.1 $(privatelibdir)

install-libmxml.sl.1:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libmxml.sl.1 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so
	$(LN) libmxml.sl.1 $(BUILDROOT)$(libdir)/libmxml.sl
	$(CP) -rf $(libdir)/libmxml.s1 $(privatelibdir)



install-libmxml.1.dylib:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libmxml.1.dylib $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libmxml.dylib
	$(LN) libmxml.1.dylib $(BUILDROOT)$(libdir)/libmxml.dylib
	$(CP) -rf $(libdir)/libmxml.dylib $(privatelibdir)



#
# Uninstall everything...
#

uninstall: uninstall-$(LIBMXML) uninstall-libmxml.a
	$(RM) $(BUILDROOT)$(bindir)/mxmldoc
	$(RM) -r $(BUILDROOT)$(docdir)
	$(RM) $(BUILDROOT)$(includedir)/mxml.h
	$(RM) $(BUILDROOT)$(libdir)/pkgconfig/mxml.pc
	$(RM) $(BUILDROOT)$(mandir)/man1/mxmldoc.1
	$(RM) $(BUILDROOT)$(mandir)/man3/mxml.3

uninstall-libmxml.a:
	$(RM) $(BUILDROOT)$(libdir)/libmxml.a
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.a


uninstall-libmxml.so.1.4:
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so.1
	$(RM) $(BUILDROOT)$(libdir)/libmxml.so.1.4
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.so
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.so.1
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.so.1.4


uninstall-libmxml.sl.1:
	$(RM) $(BUILDROOT)$(libdir)/libmxml.sl
	$(RM) $(BUILDROOT)$(libdir)/libmxml.sl.1
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.sl
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.sl.1

uninstall-libmxml.1.dylib:
	$(RM) $(BUILDROOT)$(libdir)/libmxml.dylib
	$(RM) $(BUILDROOT)$(libdir)/libmxml.1.dylib
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.dylib
	$(RM) $(BUILDROOT)$(privatelibdir)/libmxml.1.dylib


#
# Make packages using EPM (http://www.easysw.com/epm/)
#

epm:	all
	epm --output-dir dist -v -f native mxml
	epm --output-dir dist -v -f portable mxml


#
# autoconf stuff...
#

Makefile:	configure Makefile.in
	#if test -f config.status; then \
		./config.status --recheck; \
		./config.status; \
	else \
		./configure; \
	fi
	#touch config.h


config.h:	configure config.h.in
	#autoconf
	#if test -f config.status; then \
		./config.status --recheck; \
		./config.status; \
	else \
		./configure; \
	fi
	#touch config.h


#
# libmxml.a
#

libmxml.a:	$(LIBOBJS)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(LIBOBJS)
	$(RANLIB) $@

$(LIBOBJS):	mxml.h
mxml-entity.o mxml-file.o mxml-private.o: mxml-private.h


#
# libmxml.so.1.4
#

libmxml.so.1.4:	$(LIBOBJS)
	$(DSO) $(DSOFLAGS) -o libmxml.so.1.4 $(LIBOBJS)
	$(RM) libmxml.so libmxml.so.1
	$(LN) libmxml.so.1.4 libmxml.so
	$(LN) libmxml.so.1.4 libmxml.so.1


#
# libmxml.sl.1
#

libmxml.sl.1:	$(LIBOBJS)
	$(DSO) $(DSOFLAGS) -o libmxml.sl.1 $(LIBOBJS)
	$(RM) libmxml.sl
	$(LN) libmxml.sl.1 libmxml.sl


#
# libmxml.1.dylib
#

libmxml.1.dylib:	$(LIBOBJS)
	$(DSO) $(DSOFLAGS) -o libmxml.1.dylib \
		-install_name $(libdir)/libmxml.dylib \
		-current_version 1.4.0 \
		-compatibility_version 1.0.0 \
		$(LIBOBJS)
	$(RM) libmxml.dylib
	$(LN) libmxml.1.dylib libmxml.dylib


#
# mxmldoc
#

mxmldoc:	$(LIBMXML) mxmldoc.o
	$(CC) -L. $(LDFLAGS) -o $@ mxmldoc.o -lmxml $(LIBS)

mxmldoc-static:	libmxml.a mxmldoc.o
	$(CC) $(LDFLAGS) -o $@ mxmldoc.o libmxml.a $(LIBS)

mxmldoc.o:	mxml.h


#
# testmxml
#

testmxml:	libmxml.a testmxml.o
	$(CC) $(LDFLAGS) -o $@ testmxml.o libmxml.a $(LIBS)
#	@echo Testing library...
#	./testmxml test.xml >temp1.xml 2>temp1s.xml
#	./testmxml temp1.xml >temp2.xml 2>temp2s.xml
#	@if cmp temp1.xml temp2.xml; then \
		echo Stdio file test passed!; \
		$(RM) temp2.xml temp2s.xml; \
	else \
		echo Stdio file test failed!; \
	fi
#	@if cmp temp1.xml temp1s.xml; then \
		echo String test passed!; \
		$(RM) temp1.xml temp1s.xml; \
	else \
		echo String test failed!; \
	fi
#	@if cmp test.xml test.xmlfd; then \
		echo File descriptor test passed!; \
		$(RM) test.xmlfd; \
	else \
		echo File descriptor test failed!; \
	fi

testmxml.o:	mxml.h


#
# mxml.xml
#

mxml.xml:	mxmldoc-static mxml.h $(PUBLIBOBJS:.o=.c)
	$(RM) mxml.xml
#	./mxmldoc-static mxml.xml mxml.h $(PUBLIBOBJS:.o=.c) >doc/reference.html

valgrind:	mxmldoc-static
	$(RM) valgrind.xml
	valgrind --tool=memcheck --leak-check=yes ./mxmldoc-static \
		valgrind.xml mxml.h $(PUBLIBOBJS:.o=.c) \
		>valgrind.html 2>valgrind.out


#
# doc/mxml.man
#

doc/mxml.man: mxmldoc-static mxml.xml
	$(RM) doc/mxml.man
#	./mxmldoc-static --man mxml --title "Mini-XML API" \
		--intro doc/intro.man mxml.xml >doc/mxml.man


#
# All object files depend on the makefile...
#

$(OBJS):	Makefile config.h


#
# End of "$Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/mxml-2.4/Makefile#1 $".
#
