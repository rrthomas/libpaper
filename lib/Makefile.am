# libpaper Makefile.am
#
# Copyright (c) 2021 Reuben Thomas <rrt@sc3d.org>
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

AM_CPPFLAGS = -I$(top_srcdir)/libgnu -I$(top_builddir)/libgnu -DIN_LIBRARY -DENABLE_COSTLY_RELOCATABLE

sysconf_DATA = paperspecs
EXTRA_DIST = $(sysconf_DATA)

include_HEADERS = paper.h
lib_LTLIBRARIES = libpaper.la
libpaper_la_LIBADD = $(top_builddir)/libgnu/libgnu.la $(LTLIBOBJS)
libpaper_la_LDFLAGS = $(LIBTHREAD) $(LIB_SETLOCALE) $(LIB_SETLOCALE_NULL) $(LTLIBINTL) @INTL_MACOSX_LIBS@ -no-undefined -export-symbols-regex '^paper.*|^defaultpapername$$'

if OS_WIN32
libpaper_la_LDFLAGS += -avoid-version
else
libpaper_la_LDFLAGS += -version-info $(VERSION_INFO)
endif

libpaper_la_SOURCES = libpaper.c
if OS_WIN32
libpaper_la_SOURCES += libpaper.rc
endif

libpaper.c: $(builddir)/libpaper.c.in
	sed -e 's|$$[{]prefix[}]|$(prefix)|g' $(builddir)/$@.in >$@
	chmod +x $@

DISTCLEANFILES = libpaper.c
