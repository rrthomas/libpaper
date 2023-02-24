# configure.ac for libpaper
#
# Copyright (c) 2013-2023 Reuben Thomas <rrt@sc3d.org>
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

AC_PREREQ([2.71])

dnl Initialise autoconf and automake
AC_INIT([libpaper],[2.0.9],[rrt@sc3d.org],[],[https://github.com/rrthomas/libpaper])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Release number rules:
dnl This is derived from "Versioning" chapter of info libtool documentation.
dnl Format is MAJOR:MINOR:MICRO
dnl     4a) Increment major when removing or changing interfaces.
dnl     4a) 5) Increment minor when adding interfaces.
dnl     6) Set minor to zero when removing or changing interfaces.
dnl     3) Increment micro when interfaces not changed at all,
dnl               only bug fixes or internal changes made.
dnl     4b) Set micro to zero when adding, removing or changing interfaces.

dnl First extract pieces from the version number string
LIBPAPER_MAJOR_VERSION=`echo $VERSION | awk -F. '{print $1}'`
LIBPAPER_MINOR_VERSION=`echo $VERSION | awk -F. '{print $2}'`
LIBPAPER_MICRO_VERSION=`echo $VERSION | awk -F. '{print $3}'`
LIBPAPER_VERSION=$LIBPAPER_MAJOR_VERSION.$LIBPAPER_MINOR_VERSION.$LIBPAPER_MICRO_VERSION$LIBPAPER_MICRO_VERSION_SUFFIX
LIBPAPER_VERSION_NUMBER=`expr $LIBPAPER_MAJOR_VERSION \* 1000000 + $LIBPAPER_MINOR_VERSION \* 1000 + $LIBPAPER_MICRO_VERSION`

LIBPAPER_SONUM=$LIBPAPER_MAJOR_VERSION
AGE=$LIBPAPER_MINOR_VERSION
REVISION=$LIBPAPER_MICRO_VERSION
CURRENT=`expr $LIBPAPER_SONUM + $AGE`

dnl Version info for libraries = CURRENT:REVISION:AGE
VERSION_INFO=$CURRENT:$REVISION:$AGE

AC_SUBST(VERSION_INFO)
AC_SUBST(LIBPAPER_MAJOR_VERSION)
AC_SUBST(LIBPAPER_MINOR_VERSION)
AC_SUBST(LIBPAPER_MICRO_VERSION)

dnl Check for standard build environment
AC_PROG_CC
gl_EARLY
gl_paper_EARLY
AM_PROG_AR
LT_INIT
AC_USE_SYSTEM_EXTENSIONS
gl_INIT
gl_paper_INIT

# Windows-specific checks
AC_MSG_CHECKING([for native Win32])
native_win32=no
case "$host" in
  *-*-mingw*)
    native_win32=yes
    ;;
esac
AC_MSG_RESULT([$native_win32])
AM_CONDITIONAL(OS_WIN32, test "x$native_win32" = "xyes")

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

dnl x-to-1
ac_aux_dir_abs=`cd $ac_aux_dir && pwd`
AC_PATH_PROG([PERL], [perl], [$ac_aux_dir_abs/missing perl])
dnl Set a value even if not found, so that x-to-1 gives a better error.
AC_PATH_PROG([HELP2MAN], [help2man], [help2man])
CROSS_COMPILING=$cross_compiling
AC_SUBST([CROSS_COMPILING])

dnl Create files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	lib/Makefile
	lib/libpaper.c.in
	lib/libpaper.rc
	libgnu/Makefile
	src/Makefile
	src/libgnu/Makefile
	man/Makefile
	man/paperspecs.5
	tests/Makefile
])
AC_CONFIG_FILES([build-aux/x-to-1], [chmod +x build-aux/x-to-1])
AC_OUTPUT

if test $RELOCATABLE != yes; then
  AC_MSG_WARN([Building without --enable-relocatable: the tests will not work!])
fi
