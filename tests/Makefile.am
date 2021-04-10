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
	unit-pt.sh \
	unit-pt-equals.sh \
	unit-mm.sh \
	unit-in.sh \
	unit-invalid.sh \
	all-sizes.sh \
	zero-arguments.sh \
	one-argument.sh \
	two-arguments.sh \
	paperspecs-invalid.sh \
	paper-unknown.sh \
	bad-width.sh \
	bad-height.sh \
	unknown-option.sh \
	option-requires-argument.sh \
	$(EMPTY)

RESULTS = \
	default-size-environment-expected.txt \
	default-size-user-expected.txt \
	default-size-lc_paper-expected.txt \
	default-size-system-papersize-expected.txt \
	default-size-system-paperspecs-expected.txt \
	default-size-user-paperspecs-expected.txt \
	default-size-no-default-paper-expected.txt \
	unit-pt-expected.txt \
	unit-pt-equals-expected.txt \
	unit-mm-expected.txt \
	unit-in-expected.txt \
	unit-invalid-expected.txt \
	all-sizes-expected.txt \
	zero-arguments-expected.txt \
	one-argument-expected.txt \
	two-arguments-expected.txt \
	paper-unknown-expected.txt \
	unknown-option-expected.txt \
	option-requires-argument-expected.txt \
	$(EMPTY)

EXTRA_DIST = $(SH_LOG_COMPILER) $(TESTS) $(INPUTS) $(RESULTS) test-paperspecs

AM_TESTS_ENVIRONMENT = \
	export LC_ALL=C; \
	export prefix=$(prefix); \
	export sysconfdir=$(sysconfdir); \
	export bindir=$(bindir); \
	export abs_srcdir=$(abs_srcdir); \
	export LSAN_OPTIONS=suppressions=$(abs_srcdir)/asan-suppressions.txt:fast_unwind_on_malloc=0:print_suppressions=0;
