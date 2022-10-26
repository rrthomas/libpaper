# Top-level Makefile.am
#
# Copyright (c) 2013-2021 Reuben Thomas <rrt@sc3d.org>
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

ACLOCAL_AMFLAGS = -I m4 -I src/m4
AM_DISTCHECK_CONFIGURE_FLAGS = --enable-relocatable

SUBDIRS = libgnu lib src man tests

dist_doc_DATA = README

EXTRA_DIST = m4/gnulib-cache.m4

# Ignore built files that are part of the distribution (specifically,
# man/paperconfig.8)
distcleancheck_listfiles = \
       find . -type f -exec sh -c 'test -f $(srcdir)/$$1 || echo $$1' \
	    sh '{}' ';'

release: distcheck
	git diff --exit-code && \
	git tag -a -m "Release tag" "v$(VERSION)" && \
	git push && git push --tags && \
	woger github package=$(PACKAGE) version=$(VERSION) dist_type=tar.gz
