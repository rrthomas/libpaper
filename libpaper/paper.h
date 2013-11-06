/*
 * Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 *
 * Use under the GPL version 2. You are not allowed to remove this
 * copyright notice.
 *
 */

#ifndef PAPER_H
#define PAPER_H

/*
 * systempapername() returns the preferred paper size got from either the
 *   PAPER environment variable or the /etc/papersize file. If 0 is
 *   returned, one should use the value of defaultpapername().
 *
 * paperinfo() looks from the given paper name in a table of known sizes
 *   and returns 0 if it found it; if this is the case, the width and
 *   height arguments have been set to the correct values in points (one
 *   inch contains 72 points). The case of paper size is not significant.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

struct paper;

extern int paperinit(void);
extern int paperdone(void);

extern const char* papername(const struct paper*);
extern double paperpswidth(const struct paper*);
extern double paperpsheight(const struct paper*);

extern const char* defaultpapersizefile(void);
extern const char* systempapersizefile(void);
extern const char* defaultpapername(void);
extern char* systempapername(void);
extern const struct paper* paperinfo(const char*);
extern const struct paper* paperwithsize(double pswidth, double psheight);

extern const struct paper* paperfirst(void);
extern const struct paper* paperlast(void);
extern const struct paper* papernext(const struct paper*);
extern const struct paper* paperprev(const struct paper*);

#ifdef __cplusplus
}
#endif

#endif
