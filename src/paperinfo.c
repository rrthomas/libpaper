#include <config.h>

#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <paper.h>

#include "progname.h"
#include "relocatable.h"

static void usage(const char* name)
{
    fprintf(stderr,
            "Usage: %s [-n] [-s] [-u UNIT] [-a|PAPER...]\n",
	    name);
    exit(EXIT_FAILURE);
}

#define OPT_NAME	1
#define OPT_SIZE	2
#define OPT_UNIT        4
#define OPT_ALL         8

static void printinfo(const struct paper* paper, int options, double dim)
{
    int pr = 0;

    if (options & (OPT_NAME | OPT_ALL) || options == 0) {
	printf("%s", papername(paper));
	pr = 1;
    }

    if (options & OPT_SIZE) {
	if (pr) putchar(' ');
        printf("%g %g", paperpswidth(paper) / dim, paperpsheight(paper) / dim);
    }

    putchar('\n');
}

int main(int argc, char** argv)
{
    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    const char *unit = NULL;
    double dim = 1.0;
    int c;
    unsigned options = 0;
    while ((c = getopt(argc, argv, "ansu:")) != EOF) {
	switch (c) {
        case 'a':
            options |= OPT_ALL;
            break;

        case 'n':
            options |= OPT_NAME;
            break;

        case 's':
            options |= OPT_SIZE;
            break;

        case 'u':
            options |= OPT_UNIT;
            unit = optarg;
            dim = unitfactor(unit);
            if (dim == 0) usage(program_name);
            break;

        default:
            usage(program_name);
	}
    }

    if (((options & OPT_ALL) && optind != argc) || (!(options & OPT_ALL) && optind == argc && argc > 1))
	usage(program_name);

    paperinit();

    const struct paper *pinfo = NULL;
    if (options & OPT_ALL) {
	for (const struct paper* papers = paperfirst(); papers; papers = papernext(papers))
	    printinfo(papers, options, dim);
    } else {
        if (optind < argc - 1) options |= OPT_NAME;
        for (int i = optind; i < argc; i++) {
            if ((pinfo = paperinfo(argv[i])))
                printinfo(pinfo, options, dim);
            else {
                fprintf(stderr, "%s: unknown paper `%s'\n", program_name, argv[i]);
                break;
            }
        }
        if (argc == 1)
            printinfo(paperinfo(systempapername()), options, dim);
    }

    paperdone();

    return ((options & OPT_ALL) || pinfo) ? EXIT_SUCCESS : 2;
}
