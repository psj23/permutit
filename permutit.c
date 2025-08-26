#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cltail.h"
#include "combinat.h"

#define WIDTH 80

extern int optdlm;
extern char *MAEmsg;
extern char *nfostr;

char *FIOmsg = "File I/O error.\n";

extern void clrvec(char **);

static size_t factrl(size_t);
static void bbsort(char **);
static void cleanup(char **, char **, char *, FILE *);
static void nfoscr(char *);

int main(int argc, char *argv[])
{
	size_t finele, i, j, n, nCr, nPr, r, x;
	size_t seq[MAXN];
	void (*permutit)(size_t *, size_t, size_t);
	char **args, **opts, **u, **v;
	char *s;
	FILE *f_tmp;
	int c;

	
	if (argc < 2) {
		putchar('\0');
		fputs("For more information, enter:\n", stderr);
		fprintf(stderr, "%s %c?\n", argv[0], optdlm);
		return 0;
	}
	f_tmp = NULL;
	s = NULL;
	args = opts = NULL;
	c = cltail(argc, argv, "+:CDIR:Scdir:s", &opts, &args);
	if (c == '?') {
		nfoscr(argv[0]);
		cleanup(opts, args, s, f_tmp);
		return EXIT_SUCCESS;
	}
	if (c != 0) {
		if (c != MLCERR) {
			fputs("For more information, enter:\n", stderr);
			fprintf(stderr, "%s %c?\n", argv[0], optdlm);
		}
		else
			fputs(FIOmsg, stderr);
		cleanup(opts, args, s, f_tmp);
		exit(-1);
	}
	n = 0;
	for (u = opts; *u != NULL; ++u)
		if (**u == 'I' || **u == 'i') break;
	if (*u != NULL) {
		f_tmp = fopen("tmpstdin.txt", "w+");
		if (f_tmp == NULL) {
			fprintf(stderr, "%s\n", FIOmsg);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		s = calloc(WIDTH, sizeof (*s));
		if (s == NULL) {
			fputs(MAEmsg, stderr);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		while (fgets(s, WIDTH, stdin) != NULL && *s != '\n')
			if (fputs(s, f_tmp) == EOF) break;
		if (ferror(stdin) != 0 || ferror(f_tmp) != 0) {
			fprintf(stderr, "%s\n", FIOmsg);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		rewind(f_tmp);
		while (fgets(s, WIDTH, f_tmp) != NULL)
			++n;
		if (ferror(f_tmp) != 0) {
			fputs(FIOmsg, stderr);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		free(args);
		args = calloc(n + 1, sizeof (char *));
		if (args == NULL) {
			fputs(MAEmsg, stderr);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		v = args;
		rewind(f_tmp);
		while ((s = fgets(s, WIDTH, f_tmp)) != NULL) {
			j = strlen(s);
			if (*(s + (j - 1)) == '\n')
				*(s + --j) = '\0';
			*v = calloc(j + 1, sizeof (*s));
			if (*v == NULL) {
				fputs(MAEmsg, stderr);
				cleanup(opts, args, s, f_tmp);
				exit(-1);
			}
			strcpy(*v, s);
			++v;
		}
		if (ferror(f_tmp) != 0) {
			fputs(FIOmsg, stderr);
			cleanup(opts, args, s, f_tmp);
			exit(-1);
		}
		*v = NULL;
	}
	else {
		v = args;
		while (*v != NULL) {
			++n;
			++v;
		}
	}
	if (n > MAXN) {
		fprintf(stderr, "Exceeded maximum of %lu ", MAXN);
		fprintf(stderr, "positional arguments: %lu ", n);
		fprintf(stderr, "given.\n");
		cleanup(opts, args, s, f_tmp);
		exit(-1);
	}
	r = n;
	for (u = opts; *u != NULL; ++u)
		if (**u == 'R' || **u == 'r') break;
	if (*u != NULL) {
		(*(*u + 1) != '=') ? (++*u) : (*u += 2);
		r = (size_t) atoi(*u);
	}
	if (r > n) {
		fputs("Invalid r value.\n", stderr);
		cleanup(opts, args, s, f_tmp);
		exit(-1);
	}
	else if (n == 0 || r == 0) {
		putchar('\0');
		cleanup(opts, args, s, f_tmp);
		return 0;
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'S' || **u == 's') break;
	if (*u != NULL) bbsort(args);
	nPr = factrl(n) / factrl(n - r);
	nCr = nPr / factrl(r);
	for (u = opts; *u != NULL; ++u)
		if (**u == 'C' || **u == 'c') break;
	if (*u == NULL) {
		for (j = n - 1, i = 0; i < r; --j, ++i)
			*(seq + i) = j;
		permutit = permnext;
		x = nPr;
	}
	else {
		for (j = n - r, i = 0; i < r; ++j, ++i)
			*(seq + i) = j;
		permutit = combnext;
		x = nCr;
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'D' || **u == 'd') break;
	if (*u == NULL)
		(f_tmp == NULL) ? (c = ' ') : (c = '\n');
	else
		(f_tmp == NULL) ? (c = '\n') : (c = ' ');
	i = 0;
	finele = r - 1;
	do {
		permutit(seq, n, r);
		for (j = 0; j < r; ++j) {
			fputs(*(args + *(seq + j)), stdout);
			if (j < finele)
				putchar(c);
			else {
				putchar('\n');
				if (*u == NULL) {
					if (f_tmp != NULL) putchar('\n');
				}
				else
					if (f_tmp == NULL) putchar('\n');
			}
		}
		++i;
	} while (i < x);
	cleanup(opts, args, s, f_tmp);
	return EXIT_SUCCESS;
}

static size_t factrl(size_t n)
{
	size_t y;

	for (y = 1; n > 1; --n)
		y *= n;
	return y;
}

static void bbsort(char *vec[])
{
	char **u, **v;
	char *tptr;
	int sdiff, sorted;

	if (vec[0] == NULL || vec[1] == NULL)
		return;
	do {
		sorted = 1;
		v = vec;
		for (u = v, ++v; *v != NULL; ++u, ++v) {
			sdiff = strcmp(*u, *v);
			if (sdiff > 0) {
				tptr = *u;
				*u = *v;
				*v = tptr;
				sorted = 0;
			}
		}
	} while (sorted == 0);
	return;
}

static void nfoscr(char *s) {
	printf("%s <%cr[x]> <%c%c> [arg]..<argN>\n", s, optdlm, optdlm, optdlm);
	printf("\nOptions:\n");
	printf("  r : Specify an r value (where, 0 <= r <= n)\n");
	printf("  %c : End options\n", optdlm);
	printf("\nExamples:\n");
	printf("  Print all permutations of 8 objects:\n");
	printf("%s arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8\n", s);
	printf("  Print all permutations of 6 objects, 5 at a time:\n");
	printf("%s %cr5 arg1 arg2 arg3 arg4 arg5 arg6\n\n", s, optdlm);
	printf("Options, if any, precede positional arguments.\n");
	putchar('\n');
	return;
}

static void cleanup(char **opts, char **args, char *s, FILE *fp)
{
	if (opts != NULL) {
		clrvec(opts);
		free(opts);
	}
	if (args != NULL) {
		if (fp != NULL) clrvec(args);
		free(args);
	}
	if (fp != NULL) {
		fclose(fp);
		remove("tmpstdin.txt");
	}
	if (s != NULL) free(s);
	return;
}
