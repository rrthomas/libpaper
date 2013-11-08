dnl Process this file with autoconf to produce a configure script.

AC_INIT([libpaper], [1.2])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Check for standard build environment
gl_EARLY
AM_PROG_CC_C_O
AC_GNU_SOURCE
AC_PROG_INSTALL
AM_PROG_AR
AM_PROG_LIBTOOL
AC_PROG_MAKE_SET

dnl Compiler features
AC_C_CONST

AC_ARG_ENABLE([gcc-warnings],
  [AS_HELP_STRING([--enable-gcc-warnings],
                  [turn on lots of GCC warnings (for developers)])],
  [case $enableval in
     yes|no) ;;
     *)      AC_MSG_ERROR([bad value $enableval for gcc-warnings option]) ;;
   esac
   gl_gcc_warnings=$enableval],
  [gl_gcc_warnings=no]
)

if test "$gl_gcc_warnings" = yes; then
  dnl Set up the list of undesired warnings.
  nw=
  nw="$nw -Wsystem-headers"   # Don’t let system headers trigger warnings
  nw="$nw -Wundef"            # All compiler preprocessors support #if UNDEF
  nw="$nw -Wtraditional"      # All compilers nowadays support ANSI C
  nw="$nw -Wdeclaration-after-statement" # We require C99.
  nw="$nw -Wstrict-overflow"  # Use a lower level (see below).
  nw="$nw -Wconversion"       # These warnings usually don’t point to mistakes.
  nw="$nw -Wsign-conversion"  # Likewise.
  nw="$nw -Waggregate-return" # We do this on purpose.
  nw="$nw -Wbad-function-cast" # FIXME: We do this on purpose (non-pointer list elements).
  nw="$nw -Wtraditional-conversion" # Don't care.
  nw="$nw -Wpadded"           # Don't care.
  nw="$nw -Wc++-compat"       # Don't care.
  nw="$nw -Woverlength-strings" # Don't care.
  nw="$nw -Wmissing-format-attribute" # Don't care.
  nw="$nw -Wunreachable-code" # Seems buggy.

  gl_MANYWARN_ALL_GCC([warnings])

  dnl Enable all GCC warnings not in this list.
  gl_MANYWARN_COMPLEMENT([warnings], [$warnings], [$nw])
  for w in $warnings; do
    gl_WARN_ADD([$w])
  done

  dnl Add an extra warning
  gl_WARN_ADD([-Wstrict-overflow=1])
  dnl Add some more safety measures
  gl_WARN_ADD([-D_FORTIFY_SOURCE=2])
  gl_WARN_ADD([-fmudflap])

  # When compiling with GCC, prefer -isystem to -I when including system
  # include files, to avoid generating useless diagnostics for the files.
  ISYSTEM='-isystem '
else
  ISYSTEM='-I'
fi
AC_SUBST([ISYSTEM])

gl_INIT

dnl set up some substitutions
PAPERCONF="${sysconfdir}/papersize"
AC_SUBST(PAPERCONF)
AH_TEMPLATE(PAPERCONF, [Default configuration file location])

PAPERSPECS="${sysconfdir}/paperspecs"
AC_SUBST(PAPERSPECS)
AH_TEMPLATE(PAPERSPECS, [Paper specifications configuration file location])

PAPERCONFVAR=PAPERCONF
AC_SUBST(PAPERCONFVAR)
AC_DEFINE_UNQUOTED([PAPERCONFVAR], "$PAPERCONFVAR", [Environment variable that overrides the default configuration file location])

PAPERSIZE=letter
AC_SUBST(PAPERSIZE)
AC_DEFINE_UNQUOTED([PAPERSIZE], "$PAPERSIZE", [Default paper size])

RUNPARTSDIR="${sysconfdir}/libpaper.d"
AC_SUBST(RUNPARTSDIR)
AH_TEMPLATE(RUNPARTSDIR, [Default directory for run-parts])

RUNPARTSDIRVAR=PAPERDIR
AC_SUBST(RUNPARTSDIRVAR)
AC_DEFINE_UNQUOTED([RUNPARTSDIRVAR], "$RUNPARTSDIRVAR", [Environment variable that overrides the default directory for run-parts])

PAPERSIZEVAR=PAPERSIZE
AC_SUBST(PAPERSIZEVAR)
AC_DEFINE_UNQUOTED([PAPERSIZEVAR], "$PAPERSIZEVAR", [Environment variable that overrides the default paper size])

dnl Create files
AC_CONFIG_HEADER([config.h])
AC_OUTPUT([
	Makefile
	lib/Makefile
	libpaper/Makefile
	src/Makefile
	src/paperconfig
	man/Makefile
	man/paperconf.1
	man/paperconfig.8
	man/papersize.5.in
	man/systempapername.3
])
