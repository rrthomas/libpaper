# libpaper Makefile.am
#
# Copyright (c) 2021-2023 Reuben Thomas <rrt@sc3d.org>
#
# This file is part of libpaper.
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see
# <https://www.gnu.org/licenses/lgpl-2.1.html>.

include $(top_srcdir)/prefix.am

AM_CPPFLAGS = -I$(top_srcdir)/libgnu -I$(top_builddir)/libgnu -DIN_LIBRARY -DENABLE_COSTLY_RELOCATABLE -DREAL_INSTALLPREFIX="\"$(REAL_INSTALLPREFIX)\""

sysconf_DATA = paperspecs
EXTRA_DIST = $(sysconf_DATA)

include_HEADERS = paper.h
lib_LTLIBRARIES = libpaper.la
libpaper_la_LIBADD = $(top_builddir)/libgnu/libgnu.la $(LTLIBOBJS)
libpaper_la_LDFLAGS = $(LTLIBINTL) -no-undefined -export-symbols-regex '^paper.*|^defaultpapername$$|^systempapername$$|^defaultpaper$$' -version-info $(VERSION_INFO)
libpaper_la_SOURCES = libpaper.c libpaper.rc

libpaper.c: $(builddir)/libpaper.c.in
	sed -e 's|$$[{]prefix[}]|$(prefix)|g' $(builddir)/libpaper.c.in > $@-t && mv $@-t $@

DISTCLEANFILES = libpaper.c
