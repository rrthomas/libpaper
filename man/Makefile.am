man_MANS = \
	paper.1 \
	paperspecs.5

EXTRA_DIST = \
	paper-include.man.in \
	paperspecs.5.in

DISTCLEANFILES = $(man_MANS)

do_subst = sed -e 's|@sysconfdir[@]|$(sysconfdir)|'g \
	    -e 's|@PAPERCONF[@]|$(PAPERCONF)|'g

paper-include.man: paper-include.man.in Makefile
	$(do_subst) < $(srcdir)/$@.in > $@

paper.1: $(abs_top_builddir)/src/paper $(builddir)/paper-include.man
## Exit gracefully if $@ is not writeable, such as during distcheck!
	$(AM_V_GEN)if ( touch $@.w && rm -f $@.w; ) >/dev/null 2>&1; then \
	  $(top_srcdir)/build-aux/missing --run $(HELP2MAN) --no-info \
		--no-discard-stderr \
		--include=$(builddir)/paper-include.man \
		--output=$@ $(abs_top_builddir)/src/paper; \
	fi
