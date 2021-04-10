# Top-level Makefile.am
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

ACLOCAL_AMFLAGS = -I m4 -I src/m4

SUBDIRS = libgnu lib src man tests

dist_doc_DATA = README

EXTRA_DIST = m4/gnulib-cache.m4

release: distcheck
	git diff --exit-code && \
	git tag -a -m "Release tag" "v$(VERSION)" && \
	git push && git push --tags && \
	woger github package=$(PACKAGE) version=$(VERSION) dist_type=tar.gz
