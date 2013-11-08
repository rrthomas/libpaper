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

man_MANS = \
	$(raw_MANS) \
	paperconf.1 \
	systempapername.3 \
	papersize.5 \
	paperconfig.8

EXTRA_DIST = \
	$(raw_MANS) \
	paperconf.1.in \
	systempapername.3.in \
	papersize.5.in.in \
	paperconfig.8.in

DISTCLEANFILES = papersize.5

papersize.5: $(builddir)/papersize.5.in
	rm -f $@ $@.tmp
	paperspecs="`cut -d ' ' -f 1 < $(top_srcdir)/libpaper/paperspecs | tr '\n' ' ' | sed -e 's/ /, /g;s/, $$//'`" && \
		sed -e "s|@paperspecs[@]|$$paperspecs|g" $(builddir)/$@.in >$@.tmp
	mv $@.tmp $@
