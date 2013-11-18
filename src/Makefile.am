AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

bin_PROGRAMS	= paperconf
paperconf_LDADD		= $(top_builddir)/libpaper/libpaper.la $(top_builddir)/lib/libgnu.la
paperconf_CPPFLAGS	= -I$(top_srcdir)/lib -DINSTALLDIR=\"$(bindir)\"
if RELOCATABLE_VIA_LD
paperconf_LDFLAGS	= `$(RELOCATABLE_LDFLAGS) $(bindir)`
endif

sbin_SCRIPTS	= paperconfig

EXTRA_DIST	= paperconfig.in
