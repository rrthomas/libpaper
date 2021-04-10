# configure.ac for paper
#
# Copyright (c) 2013-2021 Reuben Thomas <rrt@sc3d.org>
#
# This file is part of Paper.
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

AC_PREREQ([2.64])

dnl Initialise autoconf and automake
AC_INIT([paper], [2.4], [rrt@sc3d.org],, [https://github.com/rrthomas/paper])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Check for standard build environment
AC_PROG_CC_C99
gl_EARLY
gl_paper_EARLY
AM_PROG_AR
LT_INIT
AC_GNU_SOURCE
gl_INIT
gl_paper_INIT

dnl set up some substitutions
PAPERSIZE="papersize"
AC_SUBST(PAPERSIZE)
AH_TEMPLATE(PAPERSIZE, [Default paper size configuration file name])

dnl Relocation support
AM_CONDITIONAL([SHLIBS_IN_BINDIR],
               [case "$host_os" in mingw* | cygwin*) true;; *) false;; esac])

dnl Extra warnings with GCC
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
  gl_WARN_ADD([-Wconversion])
  dnl Remove a warning being promoted to error: we trigger this and can't turn it off with pragmas.
  gl_WARN_ADD([-Wno-error=format-security])

  # When compiling with GCC, prefer -isystem to -I when including system
  # include files, to avoid generating useless diagnostics for the files.
  ISYSTEM='-isystem '
else
  ISYSTEM='-I'
fi
AC_SUBST([ISYSTEM])

dnl help2man
dnl Set a value even if not found, so that an invocation via build-aux/missing works
AC_PATH_PROG([HELP2MAN], [help2man], [help2man])

dnl Create files
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
	Makefile
	lib/Makefile
	lib/libpaper.c.in
	libgnu/Makefile
	src/Makefile
	src/libgnu/Makefile
	man/Makefile
	man/paperspecs.5
	tests/Makefile
])
AC_CONFIG_FILES([src/paperconfig], [chmod +x src/paperconfig])
AC_OUTPUT

if test $RELOCATABLE != yes; then
  AC_MSG_WARN([Building without --enable-relocatable: the tests will not work!])
fi
