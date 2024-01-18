# Man pages Makefile.am
#
# Copyright (c) 2013-2024 Reuben Thomas <rrt@sc3d.org>
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

man_MANS = paper.1 paperspecs.5

EXTRA_DIST = \
	paper.x \
	paper.1.in \
	paperspecs.5.in

DISTCLEANFILES = $(man_MANS)

do_subst = sed -e 's|@sysconfdir[@]|$(sysconfdir)|'g

paper.1: paper.1.in Makefile
	$(do_subst) < $(srcdir)/$@.in > $@-t && mv $@-t $@

paperspecs.5: paperspecs.5.in Makefile
	$(do_subst) < $(srcdir)/$@.in > $@-t && mv $@-t $@

paper.1.in: $(abs_top_srcdir)/src/paper.c $(abs_top_srcdir)/src/tbl_opts.h paper.x
## Exit gracefully if $@ is not writeable, such as during distcheck!
	$(AM_V_GEN)if ( touch $@.w && rm -f $@.w; ) >/dev/null 2>&1; then \
	  $(top_builddir)/build-aux/x-to-1 \
		$(PERL) $(HELP2MAN) --no-info --no-discard-stderr \
		$(abs_top_builddir)/src/paper$(EXEEXT) paper.x $@; \
	fi
