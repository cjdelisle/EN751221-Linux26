# Makefile for the second extended file system utility functions
#
# Copyright (C) 1993 Remy Card (card@masi.ibp.fr)
#
# This file can be redistributed under the terms of the GNU General
# Public License

srcdir = .
top_srcdir = ../..

top_builddir = ../..
my_dir = lib/e2p
INSTALL = /usr/bin/install -c

# Beginning of file MCONFIG

all::

check::

SHELL = /bin/sh

COMPRESS_EXT = gz bz2 bz Z

prefix = /proj/mtk11340/Code1/newkernel_0420/BBN_Linux/DEV/main/tclinux_phoenix/apps/public/mtd-utils/install_e2fs
root_prefix = /proj/mtk11340/Code1/newkernel_0420/BBN_Linux/DEV/main/tclinux_phoenix/apps/public/mtd-utils/install_e2fs
exec_prefix = ${prefix}
root_bindir = ${exec_prefix}/bin
root_sbindir = ${exec_prefix}/sbin
root_libdir = ${exec_prefix}/lib
datarootdir = ${prefix}/share
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libdir = ${exec_prefix}/lib
datadir= ${datarootdir}
localedir = $(datadir)/locale
root_sysconfdir= ${prefix}/etc
includedir = ${prefix}/include
mandir = ${datarootdir}/man
man1dir = $(mandir)/man1
man3dir = $(mandir)/man3
man5dir = $(mandir)/man5
man8dir = $(mandir)/man8
infodir = ${datarootdir}/info
datadir = ${datarootdir}
pkgconfigdir = $(libdir)/pkgconfig



 V =
 ifeq ($(strip $(V)),)
 #  E = @echo
 #  ES = echo
 #  Q = @
    E = @echo
    ES = echo
    Q = @
 else
    E = @\#
    ES = \#
    Q =
 endif

# E = @echo
# ES = echo
# Q = @

 CHECK=sparse
 CHECK_OPTS=-Wsparse-all -Wno-transparent-union -Wno-return-void -Wno-undef -Wno-non-pointer-null
 ifeq ("$(C)", "2")
   CHECK_CMD=$(CHECK) $(CHECK_OPTS) -Wbitwise -D__CHECK_ENDIAN__
 else
   ifeq ("$(C)", "1")
     CHECK_CMD=$(CHECK) $(CHECK_OPTS)
    else
     CHECK_CMD=@true
   endif
 endif

# CHECK_CMD=@true

CC = mips-unknown-linux-uclibc-gcc -mips32r2 -msoft-float
BUILD_CC = gcc
CFLAGS = -g -O2 -DHAVE_CONFIG_H
CPPFLAGS = -I. -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_builddir)/intl -I$(top_srcdir)/intl
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS)
LDFLAGS = 
ALL_LDFLAGS = $(LDFLAGS) 
LDFLAGS_STATIC = $(LDFLAGS) -static
BUILD_CFLAGS = 
BUILD_LDFLAGS = 
RDYNAMIC = -rdynamic
LINK_BUILD_FLAGS = 
LINK_INSTALL_FLAGS = -f
RM = /bin/rm
LN = /bin/ln
LN_S = ln -s
MV = /bin/mv
CP = /bin/cp
CHMOD = /bin/chmod
AR = mips-unknown-linux-uclibc-ar
AWK = gawk
SED = /bin/sed
PERL = /usr/local/bin/perl
RANLIB = mips-unknown-linux-uclibc-ranlib
STRIP = mips-unknown-linux-uclibc-strip
LD = $(PURE) mips-unknown-linux-uclibc-gcc -mips32r2 -msoft-float
ARUPD = $(AR) r
ARGEN = $(AR) rc
LDCONFIG = /opt/trendchip/mips-linux-uclibc-4.3.6-v2/usr/bin/ldconfig
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
MKINSTALLDIRS = $(top_builddir)/config/mkinstalldirs

#
# Library definitions
#
LIB = $(top_builddir)/lib
LIBSS = $(LIB)/libss.a  -ldl
LIBCOM_ERR = $(LIB)/libcom_err.a  -lpthread
LIBE2P = $(LIB)/libe2p.a
LIBEXT2FS = $(LIB)/libext2fs.a
LIBUUID = $(LIB)/libuuid.a 
LIBQUOTA = $(LIB)/libquota.a
LIBBLKID = $(LIB)/libblkid.a  $(LIBUUID)
LIBINTL = ${top_builddir}/intl/libintl.a  
SYSLIBS = 
DEPLIBSS = $(LIB)/libss.a
DEPLIBCOM_ERR = $(LIB)/libcom_err.a
DEPLIBUUID = $(LIB)/libuuid.a
DEPLIBQUOTA = $(LIB)/libquota.a
DEPLIBBLKID = $(LIB)/libblkid.a  $(DEPLIBUUID)
TESTENV = LD_LIBRARY_PATH="$(LIB):$${LD_LIBRARY_PATH}" DYLD_LIBRARY_PATH="$(LIB):$${DYLD_LIBRARY_PATH}"

STATIC_LIBSS = $(LIB)/libss.a -ldl
STATIC_LIBCOM_ERR = $(LIB)/libcom_err.a -lpthread
STATIC_LIBE2P = $(LIB)/libe2p.a
STATIC_LIBEXT2FS = $(LIB)/libext2fs.a
STATIC_LIBUUID = $(LIB)/libuuid.a 
STATIC_LIBQUOTA = $(LIB)/libquota.a
STATIC_LIBBLKID = $(LIB)/libblkid.a $(STATIC_LIBUUID)
DEPSTATIC_LIBSS = $(LIB)/libss.a
DEPSTATIC_LIBCOM_ERR = $(LIB)/libcom_err.a
DEPSTATIC_LIBUUID = $(LIB)/libuuid.a
DEPSTATIC_LIBQUOTA = $(LIB)/libquota.a
DEPSTATIC_LIBBLKID = $(LIB)/libblkid.a $(DEPSTATIC_LIBUUID)

PROFILED_LIBSS = $(LIB)/libss.a -ldl
PROFILED_LIBCOM_ERR = $(LIB)/libcom_err.a -lpthread
PROFILED_LIBE2P = $(LIB)/libe2p.a
PROFILED_LIBEXT2FS = $(LIB)/libext2fs.a
PROFILED_LIBUUID = $(LIB)/libuuid.a 
PROFILED_LIBQUOTA = $(LIB)/libquota.a
PROFILED_LIBBLKID = $(LIB)/libblkid.a $(PROFILED_LIBUUID)
DEPPROFILED_LIBSS = $(LIB)/libss.a
DEPPROFILED_LIBCOM_ERR = $(LIB)/libcom_err.a
DEPPROFILED_LIBUUID = $(LIB)/libuuid.a
DEPPROFILED_LIBQUOTA = $(LIB)/libquota.a
DEPPROFILED_LIBBLKID = $(LIB)/libblkid.a $(DEPPROFILED_LIBUUID)

#
# A fast substitution command for fixing up man pages, shell scripts, etc.
#
SUBST_CONF=$(top_builddir)/util/subst.conf
SUBSTITUTE= $(top_builddir)/util/subst -f $(SUBST_CONF)
SUBSTITUTE_UPTIME= $(top_builddir)/util/subst -t -f $(SUBST_CONF)
DEP_SUBSTITUTE= $(top_builddir)/util/subst $(SUBST_CONF)

$(top_builddir)/util/subst:
	cd $(top_builddir)/util ; $(MAKE) subst

#
# Script for installing symlinks (for shared libraries)
#
$(top_builddir)/util/install-symlink: $(top_srcdir)/util/install-symlink.in \
			      $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=util/install-symlink ./config.status
	chmod +x $(top_builddir)/util/install-symlink

$(top_builddir)/util/symlinks:
	cd $(top_builddir)/util ; $(MAKE) symlinks

INSTALL_SYMLINK = /bin/sh $(top_builddir)/util/install-symlink \
			 \
			--symlinks=$(top_builddir)/util/symlinks
DEP_INSTALL_SYMLINK = $(top_builddir)/util/install-symlink \
			$(top_builddir)/util/symlinks

#
# Warning flags
#
# Run make gcc-wall to do a build with warning messages.
#
#
WFLAGS=		-std=c99 -D_XOPEN_SOURCE=600 -D_GNU_SOURCE \
			-pedantic $(WFLAGS_EXTRA) \
			-Wall -W -Wwrite-strings -Wpointer-arith \
			-Wcast-qual -Wcast-align -Wno-variadic-macros \
			-Wstrict-prototypes -Wmissing-prototypes \
			-Wformat-security  -Wformat-nonliteral \
			-Wmissing-format-attribute -O2 -Wstrict-aliasing \
			-Wnested-externs -Winline -DNO_INLINE_FUNCS -Wshadow \
			-UENABLE_NLS

gcc-wall-new:
	(make CFLAGS="-g -O2 $(WFLAGS)" > /dev/null) 2>&1 | sed -f $(top_srcdir)/util/gcc-wall-cleanup 

gcc-wall:
	make clean > /dev/null
	make gcc-wall-new

#
# Installation user and groups
#
BINGRP=		bin
BINOWN=		bin
BINMODE=	555
INCGRP=		bin
INCOWN=		bin
INCMODE=	444
LIBOWN=		bin
LIBGRP=		bin
LIBMODE=	444
MANGRP=		bin
MANOWN=		bin
MANMODE=	444

#
# Autoconf magic...
#

DEP_LIB_MAKEFILES = $(top_srcdir)/lib/Makefile.library \
	$(top_srcdir)/lib/Makefile.elf-lib  \
	$(top_srcdir)/lib/Makefile.bsd-lib \
	$(top_srcdir)/lib/Makefile.darwin-lib \
	$(top_srcdir)/lib/Makefile.solaris-lib \
	$(top_srcdir)/lib/Makefile.profile

$(top_builddir)/config.status: $(top_srcdir)/configure
	cd $(top_builddir); ./config.status --recheck

$(top_builddir)/MCONFIG: $(top_srcdir)/MCONFIG.in $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=MCONFIG ./config.status

$(top_builddir)/lib/config.h: $(top_srcdir)/lib/config.h.in \
			      $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=lib/config.h ./config.status

$(top_builddir)/lib/dirpaths.h: $(DEP_SUBSTITUTE) $(top_srcdir)/lib/dirpaths.h.in
	$(E) "	SUBST $@"
	$(Q) $(SUBSTITUTE) $(top_srcdir)/lib/dirpaths.h.in $@

$(top_builddir)/lib/substitute_sh: $(top_srcdir)/lib/substitute_sh.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=lib/substitute_sh ./config.status

$(top_builddir)/util/subst.conf: $(top_srcdir)/util/subst.conf.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=util/subst.conf ./config.status

Makefile: $(srcdir)/Makefile.in $(top_builddir)/MCONFIG \
		$(DEP_MAKEFILE) $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=$(my_dir)/Makefile ./config.status

#$(top_srcdir)/configure: $(top_srcdir)/configure.in
#	cd $(top_srcdir) && autoheader && autoconf

coverage.txt: Makefile $(SRCS)
	if test -n "$(SRCS)"; then \
		gcov -s $(top_srcdir) -o . $(SRCS) > coverage.txt 2>&1 ; \
	fi

clean::
	$(RM) -f *.gcda *.gcov *.gcno coverage.txt

#
# Make depend magic...
#

.depend: Makefile $(SRCS) $(top_srcdir)/depfix.sed $(top_srcdir)/wordwrap.pl
	if test -n "$(SRCS)" ; then \
		$(CC) -M $(ALL_CFLAGS) $(DEPEND_CFLAGS) $(SRCS) | \
			$(SED) -f $(top_srcdir)/depfix.sed \
			    -e 's; $(srcdir)/; $$(srcdir)/;g' \
			    -e 's; $(top_srcdir)/; $$(top_srcdir)/;g' \
			    -e 's; $(top_builddir)/; $$(top_builddir)/;g' \
			    -e 's; \./; ;g' \
			    -e '/^#/d' \
			    -e '/^ *\\$$/d' | \
			$(PERL) $(top_srcdir)/wordwrap.pl > .depend; \
	else :; fi

depend:: .depend
	if test -n "$(SRCS)" ; then \
		sed -e '/^# +++ Dependency line eater +++/,$$d' \
			< $(srcdir)/Makefile.in | cat - .depend \
			> $(srcdir)/Makefile.in.new; \
	if cmp -s $(srcdir)/Makefile.in $(srcdir)/Makefile.in.new ; then \
		$(RM) $(srcdir)/Makefile.in.new ; \
	else \
		$(MV) $(srcdir)/Makefile.in $(srcdir)/Makefile.in.old; \
		$(MV) $(srcdir)/Makefile.in.new $(srcdir)/Makefile.in; \
	fi ; else :; fi

# End of file MCONFIG

all::	e2p.pc

OBJS=		feature.o fgetflags.o fsetflags.o fgetversion.o fsetversion.o \
		getflags.o getversion.o hashstr.o iod.o ls.o mntopts.o \
		parse_num.o pe.o pf.o ps.o setflags.o setversion.o uuid.o \
		ostype.o percent.o

SRCS=		$(srcdir)/feature.c $(srcdir)/fgetflags.c \
		$(srcdir)/fsetflags.c $(srcdir)/fgetversion.c \
		$(srcdir)/fsetversion.c $(srcdir)/getflags.c \
		$(srcdir)/getversion.c $(srcdir)/hashstr.c $(srcdir)/iod.c \
		$(srcdir)/ls.c $(srcdir)/mntopts.c $(srcdir)/parse_num.c \
		$(srcdir)/pe.c $(srcdir)/pf.c $(srcdir)/ps.c \
		$(srcdir)/setflags.c $(srcdir)/setversion.c $(srcdir)/uuid.c \
		$(srcdir)/ostype.c $(srcdir)/percent.c
HFILES= e2p.h

LIBRARY= libe2p
LIBDIR= e2p

ELF_VERSION = 2.3
ELF_SO_VERSION = 2
ELF_IMAGE = libe2p
ELF_MYDIR = e2p
ELF_INSTALL_DIR = $(root_libdir)
ELF_OTHER_LIBS =

BSDLIB_VERSION = 2.1
BSDLIB_IMAGE = libe2p
BSDLIB_MYDIR = e2p
BSDLIB_INSTALL_DIR = $(root_libdir)

all:: subdirs $(LIBRARY).a

install-shlibs::

uninstall-shlibs::

real-subdirs::

subdirs:: Makefile
	$(Q) $(MAKE) -s real-subdirs
	$(Q) touch subdirs

clean::
	$(Q) $(RM) -f subdirs

$(LIBRARY).a: $(OBJS)
	$(E) "	GEN_LIB $@"
	$(Q) (if test -r $@; then $(RM) -f $@.bak && $(MV) $@ $@.bak; fi)
	$(Q) $(ARGEN) $@ $(OBJS)
	-@$(RANLIB) $@
	$(Q) $(RM) -f ../$@
	$(Q) (cd ..; $(LN) $(LINK_BUILD_FLAGS) \
		`echo $(my_dir) | sed -e 's;lib/;;'`/$@ $@)

$(LIB)/$(LIBRARY).a: $(LIBRARY).a

.c.o:
	$(E) "	CC $<"
	$(Q) $(CC) $(ALL_CFLAGS) -c $< -o $@
	$(Q) $(CHECK_CMD) $(ALL_CFLAGS) $<
#	$(Q) $(CC) $(ALL_CFLAGS) -g -pg -o profiled/$*.o -c $<
#	$(Q) $(CC) $(ALL_CFLAGS) -fPIC -o elfshared/$*.o -c $<
#	$(Q) $(CC) $(ALL_CFLAGS) $(BSDLIB_PIC_FLAG) -o pic/$*.o -c $<

e2p.pc: $(srcdir)/e2p.pc.in $(top_builddir)/config.status
	$(E) "	CONFIG.STATUS $@"
	$(Q) cd $(top_builddir); CONFIG_FILES=lib/e2p/e2p.pc ./config.status

tst_ostype: $(srcdir)/ostype.c
	$(E) "	LD $@"
	$(Q) $(CC) -DTEST_PROGRAM -I$(top_srcdir)/lib -o tst_ostype \
		$(srcdir)/ostype.c $(ALL_CFLAGS)

tst_feature: $(srcdir)/feature.c
	$(E) "	LD $@"
	$(Q) $(CC) -DTEST_PROGRAM -I$(top_srcdir)/lib -o tst_feature \
		$(srcdir)/feature.c $(ALL_CFLAGS)

check::	tst_ostype tst_feature
	./tst_ostype
	./tst_feature

installdirs::
	$(E) "	MKINSTALLDIRS $(libdir) $(includedir)/e2p"
	$(Q) $(MKINSTALLDIRS) $(DESTDIR)$(libdir) \
		$(DESTDIR)$(includedir)/e2p $(DESTDIR)$(pkgconfigdir)

install:: all installdirs 
	$(E) "	INSTALL_DATA $(libdir)/libe2p.a"
	$(Q) $(INSTALL_DATA) libe2p.a $(DESTDIR)$(libdir)/libe2p.a
	-$(Q) $(RANLIB) $(DESTDIR)$(libdir)/libe2p.a
	$(Q) $(CHMOD) $(LIBMODE) $(DESTDIR)$(libdir)/libe2p.a
	$(Q) set -e; for i in $(HFILES); do \
	  echo "	INSTALL_DATA $(includedir)/e2p/$$i"; \
	  $(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(includedir)/e2p/$$i; \
	done
	$(E) "	INSTALL_DATA $(pkgconfigdir)/e2p.pc"
	$(Q) $(INSTALL_DATA) e2p.pc $(DESTDIR)$(pkgconfigdir)/e2p.pc

uninstall::
	$(RM) -f $(DESTDIR)$(libdir)/libe2p.a \
		$(DESTDIR)$(pkgconfigdir)/e2p.pc
	$(RM) -rf $(DESTDIR)$(includedir)/e2p

clean::
	$(RM) -f \#* *.s *.o *.a *~ *.bak core profiled/*
	$(RM) -f ../libe2p.a ../libe2p_p.a tst_ostype tst_feature

mostlyclean:: clean
distclean:: clean
	$(RM) -f .depend Makefile e2p.pc \
		$(srcdir)/TAGS $(srcdir)/Makefile.in.old

$(OBJS): subdirs

# +++ Dependency line eater +++
# 
# Makefile dependencies follow.  This must be the last section in
# the Makefile.in file
#
feature.o: $(srcdir)/feature.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h \
 $(top_srcdir)/lib/ext2fs/ext2fs.h $(top_srcdir)/lib/ext2fs/ext3_extents.h \
 $(top_srcdir)/lib/et/com_err.h $(top_srcdir)/lib/ext2fs/ext2_io.h \
 $(top_builddir)/lib/ext2fs/ext2_err.h \
 $(top_srcdir)/lib/ext2fs/ext2_ext_attr.h $(top_srcdir)/lib/ext2fs/bitops.h \
 $(top_srcdir)/lib/ext2fs/jfs_user.h $(top_srcdir)/lib/ext2fs/kernel-jbd.h \
 $(top_srcdir)/lib/ext2fs/jfs_compat.h $(top_srcdir)/lib/ext2fs/kernel-list.h
fgetflags.o: $(srcdir)/fgetflags.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
fsetflags.o: $(srcdir)/fsetflags.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
fgetversion.o: $(srcdir)/fgetversion.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
fsetversion.o: $(srcdir)/fsetversion.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
getflags.o: $(srcdir)/getflags.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
getversion.o: $(srcdir)/getversion.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
hashstr.o: $(srcdir)/hashstr.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
iod.o: $(srcdir)/iod.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
ls.o: $(srcdir)/ls.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
mntopts.o: $(srcdir)/mntopts.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
parse_num.o: $(srcdir)/parse_num.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
pe.o: $(srcdir)/pe.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
pf.o: $(srcdir)/pf.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
ps.o: $(srcdir)/ps.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
setflags.o: $(srcdir)/setflags.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
setversion.o: $(srcdir)/setversion.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
uuid.o: $(srcdir)/uuid.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(top_builddir)/lib/ext2fs/ext2_types.h \
 $(srcdir)/e2p.h $(top_srcdir)/lib/ext2fs/ext2_fs.h
ostype.o: $(srcdir)/ostype.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
percent.o: $(srcdir)/percent.c $(top_builddir)/lib/config.h \
 $(top_builddir)/lib/dirpaths.h $(srcdir)/e2p.h \
 $(top_srcdir)/lib/ext2fs/ext2_fs.h $(top_builddir)/lib/ext2fs/ext2_types.h
