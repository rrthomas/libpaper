AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

bin_PROGRAMS	= paperinfo
paperinfo_LDADD		= $(top_builddir)/lib/libgnu.a -lm
paperinfo_CPPFLAGS	= $(ISYSTEM)$(top_srcdir)/lib -DINSTALLDIR=\"$(bindir)\" -DPAPERCONF='"$(PAPERCONF)"' -DRUNPARTSDIR='"$(RUNPARTSDIR)"' -DPAPERSPECS='"$(PAPERSPECS)"'
if RELOCATABLE_VIA_LD
paperinfo_LDFLAGS	= `$(RELOCATABLE_LDFLAGS) $(bindir)`
endif

bin_SCRIPTS	= paperconfig
sysconf_DATA	= paperspecs

EXTRA_DIST	= paperconfig.in $(sysconf_DATA)
