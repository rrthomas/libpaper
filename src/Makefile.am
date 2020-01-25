libexec_PROGRAMS= localepaper
LDADD		= $(top_builddir)/lib/libgnu.a
AM_CPPFLAGS	= -I$(top_srcdir)/lib

bin_SCRIPTS	= paper
sysconf_DATA	= paperspecs

EXTRA_DIST	= $(sysconf_DATA)

DISTCLEANFILES	= $(bin_SCRIPTS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g' -e 's|$$[{]exec_prefix[}]|$(exec_prefix)|g'

paper: $(builddir)/paper.in
	$(edit) $(builddir)/$@.in >$@
	chmod +x $@

install-exec-hook:
	if test "x$(EXEEXT)" != "x"; then \
	  mv $(DESTDIR)$(bindir)/paper $(DESTDIR)$(bindir)/paper$(PERL_EXEEXT); \
	fi
