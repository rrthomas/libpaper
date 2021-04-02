# Tests Makefile.am
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

TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(srcdir)/run-test

EMPTY =

TESTS = \
	default-size-environment.sh \
	default-size-user.sh \
	default-size-lc_paper.sh \
	default-size-system-papersize.sh \
	default-size-system-paperspecs.sh \
	default-size-user-paperspecs.sh \
	default-size-no-default-paper.sh \
	$(EMPTY)

XFAIL_TESTS = \
	$(EMPTY)

INPUTS = \
	$(EMPTY)

RESULTS = \
	$(EMPTY)

EXTRA_DIST = $(SH_LOG_COMPILER) $(TESTS) $(INPUTS) $(RESULTS)

AM_TESTS_ENVIRONMENT = \
	export prefix=$(prefix); \
	export sysconfdir=$(sysconfdir); \
	export bindir=$(bindir); \
	export abs_srcdir=$(abs_srcdir); \
	export LSAN_OPTIONS=suppressions=$(abs_srcdir)/asan-suppressions.txt:fast_unwind_on_malloc=0;

clean-local:
	rm -f $(builddir)/*-output.log
