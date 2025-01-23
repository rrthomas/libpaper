# libpaper

© Reuben Thomas <rrt@sc3d.org>, 2013-2025  
https://github.com/rrthomas/libpaper  

The libpaper package enables users to indicate their preferred paper
size, provides the paper utility to find the user's preferred default
paper size and give information about known sizes, and specifies
system-wide and per-user paper size catalogues (see “man paperspecs”).

See “man paper” for information about how to set the default paper size.

The libpaper package is distributed under the MIT license (if built without
`--enable-relocatable`; see the file COPYING-MIT), or the GNU Lesser General
Public License, version 2.1, or, at your option, any later version (if built
with `--enable-relocatable`; see the file COPYING), with the exception of
the files listed below.

* The file `paperspecs` is in the public domain.
* The `paper` program is distributed under the GNU General Public Licence
  version 3, or, at your option, any later version. See the file
  COPYING-GPL-3.
* The `paperconf` program by Yyes Arrouye and Adrian Bunk is distributed
  under the GNU General Public Licence version 2.

Libpaper was developed by Reuben Thomas <rrt@sc3d.org>, based on the
design of libpaper by Yves Arrouye <yves@debian.org>, improved by
Adrian Bunk <bunk@fs.tum.de>. It also supersedes the earlier “paper”
package developed by Reuben Thomas.

The `paperconf` program is supplied for backwards-compatibility with
libpaper version 1. It is deprecated, and will be removed in future.


# Installation from source

To build, you need a POSIX.1-2008 environment and C99 compiler.
Run:

```
./configure --enable-relocatable && make && [sudo] make install
```

The --enable-relocatable option builds libpaper as a relocatable
package that can be copied anywhere once built and installed. Note
that if you do not use it, the tests (“make check”) will not work.

For installation options, see `./configure --help`; see INSTALL for more
information.


# Building from git

To build from git, you need the following extra programs installed:
git, automake, autoconf, help2man.

Having cloned the repository, run: `./bootstrap`, and follow the
instructions for building from source.
