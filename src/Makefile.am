# Source code Makefile.am
#
# Copyright (c) 2013-2021 Reuben Thomas <rrt@sc3d.org>
#
# This file is part of libpaper.
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

SUBDIRS = libgnu

include $(top_srcdir)/prefix.am

AM_CPPFLAGS = -I$(top_srcdir)/src/libgnu -I$(top_builddir)/src/libgnu -I$(top_srcdir)/lib -I$(top_builddir)/lib -DREAL_INSTALLPREFIX="\"$(REAL_INSTALLPREFIX)\""
AM_CFLAGS = $(WARN_CFLAGS)
RELOCATABLE_LIBRARY_PATH = $(libdir)

bin_PROGRAMS = paper
paper_SOURCES = paper.c tbl_opts.h
paper_CPPFLAGS  = -DINSTALLDIR=\"$(bindir)\" $(AM_CPPFLAGS)
if RELOCATABLE_VIA_LD
paper_LDFLAGS = `$(RELOCATABLE_LDFLAGS) $(bindir)`
endif
paper_LDADD = $(top_builddir)/lib/libpaper.la $(top_builddir)/src/libgnu/libgnupaper.la

sbin_SCRIPTS = paperconfig

EXTRA_DIST = paperconfig.in
DISTCLEANFILES = $(bin_SCRIPTS)
