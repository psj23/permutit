#include "permutit.h"

extern char *MALmsg;
extern char *nfostr;
extern int optdlm;

extern int cltail(int, char *const[], const char *, char ***, char ***);
extern void combnext(size_t *, size_t, size_t);
extern void permnext(size_t *, size_t, size_t);

static size_t factrl(size_t);
static void bbsort(char **);
static void lptnfo(char *);
static void cleanup(char **, char **, size_t, char *);

int main(int argc, char *argv[])
{
	size_t finele, i, j, n, nl, nCr, nPr, r, x;
	size_t seq[MAXN];
	void (*permutit)(size_t *, size_t, size_t);
	char **opts, **u, **args, **v;
	char buf[BUFLEN];
	int c;

	if (argc < 2) {
		fputs("For more information, enter:\n", stderr);
		fprintf(stderr, "%s -?\n", argv[0]);
		return 0;
	}
	args = opts = NULL;
	n = nl = 0;
	c = cltail(argc, argv, "+:CDIR:Scdir:s", &opts, &args);
	if (c == '?') {
		lptnfo(argv[0]);
		cleanup(opts, args, 0, NULL);
		exit((int) '?');
	}
	if (c != 0) {
		if (c != ENOMEM) {
			fputs("For more information, enter:\n", stderr);
			fprintf(stderr, "%s %c?\n", argv[0], optdlm);
		}
		cleanup(opts, args, nl, (c == ENOMEM) ? (MALmsg) : (NULL));
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'I' || **u == 'i') break;
	if (*u != NULL) {
		args = realloc(args, sizeof (char *));
		if (args == NULL) cleanup(opts, args, nl, MALmsg);
		*args = NULL;
		while (fgets(buf, BUFLEN, stdin) != NULL) {
			if (*buf == '\n') break;
			args = realloc(args, (++nl + 1) * sizeof (char *));
			if (args == NULL)
				cleanup(opts, args, nl, MALmsg);
			j = strlen(buf);
			if (j == BUFLEN - 1 || buf[j - 1] == '\n')
				buf[--j] = '\0';
			*(args + n) = calloc(j + 1, sizeof (*buf));
			if (*(args + n) == NULL)
				cleanup(opts, args, nl, MALmsg);
			strcpy(*(args + n), buf);
			*(args + ++n) = NULL;
		}
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
		fprintf(stderr, "positional arguments: %lu given.\n", n);
		cleanup(opts, args, nl, NULL);
		exit(EXIT_FAILURE);
	}
	r = n;
	for (u = opts; *u != NULL; ++u)
		if (**u == 'R' || **u == 'r') break;
	if (*u != NULL)
		r = (size_t) atoi(*u + 1);
	if (r > n) {
		fputs("Out-of-range r value.\n", stderr);
		cleanup(opts, args, nl, NULL);
		exit(EXIT_FAILURE);
	}
	else if (n == 0 || r == 0) {
		cleanup(opts, args, nl, NULL);
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
		(nl == 0) ? (c = ' ') : (c = '\n');
	else
		(nl == 0) ? (c = '\n') : (c = ' ');
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
					if (nl != 0) putchar('\n');
				}
				else {
					if (nl == 0) putchar('\n');
				}
			}
		}
		++i;
	} while (i < x);
	cleanup(opts, args, nl, NULL);
	return 0;
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

static void lptnfo(char *s) {
	fprintf(stderr, "%s <%cc> <%cr[x]> <%cs> <%c%c> [arg1]..<arg%lu>\n",\
	 s, optdlm, optdlm, optdlm, optdlm, optdlm, MAXN);
	fputc('\n', stderr);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, " c  : Print only combinations\n");
	fprintf(stderr, " r  : Specify an r value (where, 0 <= r <= n)\n");
	fprintf(stderr, " s  : Sort arguments in ascending ASCII order\n");
	fprintf(stderr, " %c  : Ends processing of options\n", optdlm);
	fputc('\n', stderr);
	fprintf(stderr, "Options, if any, precede positional arguments.\n");
	return;
}

static void cleanup(char **opts, char **args, size_t nl, char *str)
{
	char **vec;

	if (opts != NULL) {
		for (vec = opts; *vec != NULL; ++vec) free(*vec);
		free(opts);
	}
	if (args != NULL) {
		if (nl != 0) 
			for (vec = args; *vec != NULL; ++vec) free(*vec);
		free(args);
	}
	if (str != NULL) perror(str);
	if (errno != 0) exit(errno);
	else return;
}
