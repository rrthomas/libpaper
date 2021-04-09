# libpaper Makefile.am
#
# Copyright (c) 2021 Reuben Thomas <rrt@sc3d.org>
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

AM_CPPFLAGS = -I$(top_srcdir)/libgnu -I$(top_builddir)/libgnu

sysconf_DATA = paperspecs
EXTRA_DIST = $(sysconf_DATA)

include_HEADERS = paper.h
lib_LTLIBRARIES = libpaper.la
libpaper_la_SOURCES = libpaper.c
libpaper_la_LIBADD = $(top_builddir)/libgnu/libgnu.la $(LTLIBOBJS)

libpaper.c: $(builddir)/libpaper.c.in
	sed -e 's|$$[{]prefix[}]|$(prefix)|g' $(builddir)/$@.in >$@
	chmod +x $@
