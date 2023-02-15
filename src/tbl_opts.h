/* 
  SPDX-FileCopyrightText: 2021 Reuben Thomas <rrt@sc3d.org>
  SPDX-License-Identifier: GPL-3.0-or-later

  Table of command-line options
  This file is part of libpaper.

  Copyright (c) 2021 Reuben Thomas <rrt@sc3d.org>
  This file is part of libpaper.

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <https://www.gnu.org/licenses/gpl-3.0.html>.
*/

/*
 * O(long name, short name ('\0' for none), argument, argument docstring, docstring)
 */

O("all", '\0', no_argument, "", "show information about all known paper sizes")
O("no-size", '\0', no_argument, "", "do not show paper sizes (width followed by height)")
O("unit", '\0', required_argument, "UNIT", unit_msg)
O("help", '\0', no_argument, "", "display this help message and exit")
O("version", '\0', no_argument, "", "display version information and exit")
