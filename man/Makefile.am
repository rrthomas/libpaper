raw_MANS = \
	defaultpapername.3 \
	paperdone.3 \
	paperfirst.3 \
	paperinfo.3 \
	paperinit.3 \
	papernext.3 \
	paperpsheight.3 \
	paperpsname.3 \
	paperpswidth.3 \
	paperwithsize.3

generated_MANS = \
	paperconf.1 \
	systempapername.3 \
	papersize.5 \
	paperconfig.8

man_MANS = $(raw_MANS) $(generated_MANS)

EXTRA_DIST = \
	$(raw_MANS) \
	paperconf.1.in.in \
	systempapername.3.in.in \
	papersize.5.in.in \
	paperconfig.8.in.in

DISTCLEANFILES = $(generated_MANS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paperconf.1: $(builddir)/paperconf.1.in
	$(edit) $(builddir)/$@.in >$@

systempapername.3: $(builddir)/systempapername.3.in
	$(edit) $(builddir)/$@.in >$@

papersize.5: $(builddir)/papersize.5.in
	rm -f $@ $@.tmp
	paperspecslist="`cut -d ' ' -f 1 < $(top_srcdir)/libpaper/paperspecs | tr '\n' ' ' | sed -e 's/ /, /g;s/, $$//'`" && \
		sed -e "s|@paperspecslist[@]|$$paperspecslist|g" $(builddir)/$@.in >$@.tmp
	mv $@.tmp $@

paperconfig.8: $(builddir)/paperconfig.8.in
	$(edit) $(builddir)/$@.in >$@
