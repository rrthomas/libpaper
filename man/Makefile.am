# Man pages Makefile.am
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

man_MANS = \
	paper.1 \
	paperconfig.8 \
	paperspecs.5

EXTRA_DIST = \
	paper-include.man.in \
	paperspecs.5.in \
	paperconfig.8.in

DISTCLEANFILES = $(man_MANS) paper-include.man

do_subst = sed -e 's|@sysconfdir[@]|$(sysconfdir)|'g \
	    -e 's|@PAPERSIZE[@]|$(PAPERSIZE)|'g

paper-include.man: paper-include.man.in Makefile
	$(do_subst) < $(srcdir)/$@.in > $@

paperconfig.8: paperconfig.8.in Makefile
	$(do_subst) < $(srcdir)/$@.in >$@

paper.1: $(abs_top_builddir)/src/paper $(builddir)/paper-include.man
## Exit gracefully if $@ is not writeable, such as during distcheck!
	$(AM_V_GEN)if ( touch $@.w && rm -f $@.w; ) >/dev/null 2>&1; then \
	  $(top_srcdir)/build-aux/missing --run $(HELP2MAN) --no-info \
		--no-discard-stderr \
		--include=$(builddir)/paper-include.man \
		--output=$@ $(abs_top_builddir)/src/paper; \
	fi
