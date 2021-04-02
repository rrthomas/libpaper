/*
  Paper sizes command-line utility.

  Copyright (c) 2021 Reuben Thomas <rrt@sc3d.org>.

  The API is mostly API/ABI-compatible with that of the original
  libpaper by Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PAPER_H
#define PAPER_H

/* Allow the include file to be used directly from C++. */
#ifdef __cplusplus
extern "C" {
#endif

/* Opaque struct. */
struct paper;

enum paper_error {
    PAPER_OK,
    PAPER_BAD_WIDTH,
    PAPER_BAD_HEIGHT,
    PAPER_BAD_UNIT,
    PAPER_MISSING_FIELD,
    PAPER_ERROR = -1
};

extern size_t paper_lineno;
extern char *paper_specsfile;

/*
 * Initialize the library, and read configured paper sizes.
 * This function must be called before any other in the library.
 * Returns a paper_error value (PAPER_OK for success).
 */
int paperinit(void);

/*
 * Free any resources allocated by paperinit().
 * After calling it, no other library function may be called, except
 * paperinit().
 * Returns a paper_error value (PAPER_OK for success).
 */
int paperdone(void);

/*
 * Return the name of the given paper.
 */
const char *papername(const struct paper *paper);

/*
 * Return the width of the given paper in PostScript points.
 */
double paperpswidth(const struct paper *paper);

/*
 * Return the height of the given paper in PostScript points.
 */
double paperpsheight(const struct paper *paper);

/*
 * Look up a paper size by name (case-insensitive).
 * Return the paper if found, or NULL if not.
 */
const struct paper *paperinfo(const char *papername);

/*
 * Look up a paper size by size (in PostScript points).
 * Return the paper if found, or NULL if not.
 */
const struct paper *paperwithsize(double pswidth, double psheight);

/*
 * Returns the current paper size, as specified in paper(1), or NULL if none
 * can be determined.
 */
const struct paper *defaultpaper(void);

/*
 * Deprecated, only for backwards compatibility. Returns the default
 * configured paper name.
 */
const char *defaultpapername(void);

/*
 * Deprecated, only for backwards compatibility; an alias for
 * defaultpapername().
 */
#define systempapername defaultpapername

/*
 * Return the first paper in the list of known paper sizes.
 */
const struct paper *paperfirst(void);

/*
 * Return the next paper in the list of known paper sizes.
 */
const struct paper *papernext(const struct paper *paper);

double unitfactor(const char *unit);

#ifdef __cplusplus
}
#endif

#endif
