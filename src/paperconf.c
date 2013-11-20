#include <config.h>

#include <ctype.h>
#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <paper.h>

#include "progname.h"
#include "relocatable.h"

static void usage(const char* name)
{
    fprintf(stderr,
	"Usage: %s [[-p] PAPERNAME|-a] [-z] [-n|-N] [-s|-w|-h] [-c|-m|-i]\n",
	    name);
    exit(EXIT_FAILURE);
}

#define OPT_NAME	1
#define OPT_UPPERNAME	2
#define OPT_WIDTH	4
#define OPT_HEIGHT      8
#define OPT_CM         16
#define OPT_MM         32
#define OPT_INCH       64
#define OPT_CONTINUE  128

#define OPT_UNIT      (OPT_CM | OPT_MM | OPT_INCH)

#define INCHxCM 2.54

static void printinfo(const struct paper* paper, int options)
{
    int pr = 0;

    if ((options & ~(OPT_CONTINUE)) == 0) {
	options = OPT_NAME;
    }

    if (options & OPT_NAME) {
	printf("%s", papername(paper));
	pr = 1;
    } else if (options & OPT_UPPERNAME) {
	if (islower(*papername(paper))) {
	    printf("%c%s", toupper(*papername(paper)), papername(paper) + 1);
	} else {
	    printf("%s", papername(paper));
	}
	pr = 1;
    }

    if (options & OPT_WIDTH) {
	if (pr) putchar(' ');
        if (options & OPT_CM)
          printf("%g cm", paperpswidth(paper) / 72.0 * INCHxCM );
	else if (options & OPT_MM)
          printf("%g mm", paperpswidth(paper) / 72.0 * 10 * INCHxCM );
	else if (options & OPT_INCH)
          printf("%g\"", paperpswidth(paper) / 72.0 );
	else
          printf("%g", paperpswidth(paper) );
	pr = 1;
    }
    if (options & OPT_HEIGHT) {
	if (pr) putchar(' ');
        if (options & OPT_CM)
          printf("%g cm", paperpsheight(paper) / 72.0 * INCHxCM );
	else if (options & OPT_MM)
          printf("%g mm", paperpsheight(paper) / 72.0 * 10 * INCHxCM );
	else if (options & OPT_INCH)
          printf("%g\"", paperpsheight(paper) / 72.0 );
	else
          printf("%g", paperpsheight(paper) );
	pr = 1;
    }

    putchar('\n');
}

int main(int argc, char** argv)
{
    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    const char* paper = NULL;
    int c, all = 0;
    unsigned options = 0;
    while ((c = getopt(argc, argv, "aznNswhcmip:")) != EOF) {
	switch (c) {
	    case 'a':
		if (paper)
		    usage(program_name);
		all = 1;
		break;

	    case 'p':
		if (paper || all)
		    usage(program_name);
		paper = optarg;
		break;

	    case 'z':
		options |= OPT_CONTINUE;
		break;

	    case 'n':
		if (options & OPT_UPPERNAME) usage(program_name);
		options |= OPT_NAME;
		break;

	    case 'N':
		if (options & OPT_NAME) usage(program_name);
		options |= OPT_UPPERNAME;
		break;

	    case 's':
		options |= OPT_WIDTH | OPT_HEIGHT;
		break;

	    case 'w':
		options |= OPT_WIDTH;
		break;

	    case 'h':
		options |= OPT_HEIGHT;
		break;

	    case 'c':
	        if (options & OPT_UNIT) usage(program_name);
		options |= OPT_CM;
		break;

	    case 'm':
	        if (options & OPT_UNIT) usage(program_name);
		options |= OPT_MM;
		break;

	    case 'i':
	        if (options & OPT_UNIT) usage(program_name);
		options |= OPT_INCH;
		break;

	    default:
		usage(program_name);
	}
    }

    if (optind < argc - 1 || (paper && optind != argc)) {
	usage(program_name);
    } else if (optind != argc) {
	paper = argv[optind];
    }

    paperinit();

    if (all) {
	for (const struct paper* papers = paperfirst(); papers; papers = papernext(papers))
	    printinfo(papers, options);
    } else {
        if (!paper) paper = systempapername();

        const struct paper* syspaper = paperinfo(paper);
        if (syspaper) {
            printinfo(syspaper, options);
        } else {
	    fprintf(stderr, "%s: unknown paper `%s'\n", program_name, paper);
	    if (options & OPT_CONTINUE) {
		puts(paper);
	    }

	    paperdone();

	    exit(2);
        }
    }

    paperdone();

    return EXIT_SUCCESS;
}
