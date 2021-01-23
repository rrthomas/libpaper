# Source code Makefile.am
#
# Copyright (c) 2013-2021 Reuben Thomas <rrt@sc3d.org>
#
# This file is part of Paper.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <https://www.gnu.org/licenses/>.

LDADD = $(top_builddir)/lib/libgnu.a
AM_CPPFLAGS = -I$(top_srcdir)/lib
AM_CFLAGS = $(WARN_CFLAGS)

sysconf_DATA = paperspecs

RELOCATABLE_LIBRARY_PATH = $(libdir)
lib_LTLIBRARIES = libpaper.la
libpaper_la_SOURCES = lib.c
if RELOCATABLE_VIA_LD
libpaper_la_LDFLAGS = `$(RELOCATABLE_LDFLAGS) $(libdir)` $(AM_LDFLAGS)
endif
libpaper_la_LIBADD = $(top_builddir)/lib/libgnu.la $(LTLIBOBJS)

lib.c: $(builddir)/lib.c.in
	sed -e 's|$$[{]prefix[}]|$(prefix)|g' $(builddir)/$@.in >$@
	chmod +x $@

bin_PROGRAMS = paper
paper_SOURCES = paper.c tbl_opts.h
paper_CPPFLAGS  = -DINSTALLDIR=\"$(bindir)\" $(AM_CPPFLAGS)
if RELOCATABLE_VIA_LD
paper_LDFLAGS = `$(RELOCATABLE_LDFLAGS) $(bindir)` $(AM_LDFLAGS)
endif
paper_LDADD = libpaper.la $(LDADD)

EXTRA_DIST = $(sysconf_DATA)

DISTCLEANFILES = $(bin_SCRIPTS)
