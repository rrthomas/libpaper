/*
 * Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 *
 * Use under the GPL version 2. You are not allowed to remove this
 * copyright notice.
 *
 */

#ifndef PAPER_H
#define PAPER_H

#ifdef __cplusplus
extern "C" {
#endif

struct paper;

int paperinit(void);
int paperdone(void);

const char* papername(const struct paper*);
double paperpswidth(const struct paper*);
double paperpsheight(const struct paper*);

const char* systempapername(void);
const struct paper* paperinfo(const char*);
const struct paper* paperwithsize(double pswidth, double psheight);

const struct paper* paperfirst(void);
const struct paper* papernext(const struct paper*);

#ifdef __cplusplus
}
#endif

#endif
