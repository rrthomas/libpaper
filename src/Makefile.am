AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

bin_PROGRAMS	= paperconf
paperconf_LDADD		= $(top_builddir)/libpaper/libpaper.la $(top_builddir)/lib/libgnu.la
paperconf_CPPFLAGS	= -I$(top_srcdir)/lib

sbin_SCRIPTS	= paperconfig

EXTRA_DIST	= paperconfig.in
