/*
  Table of command-line options

  Copyright (c) 2021 Reuben Thomas <rrt@sc3d.org>

  This file is part of Paper.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 3, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
  Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, see <https://www.gnu.org/licenses/>.
*/

/*
 * O(long name, short name ('\0' for none), argument, argument docstring, docstring)
 */

O("all", '\0', no_argument, "", "print information about all known paper sizes")
O("name", '\0', no_argument, "", "print paper names even if only one paper argument given")
O("size", '\0', no_argument, "", "print paper sizes (width followed by height)")
O("unit", '\0', required_argument, "UNIT", "print dimensions in the given unit [default: PostScript points]")
O("help", '\0', no_argument, "", "display this help message and exit")
O("version", '\0', no_argument, "", "display version information and exit")
