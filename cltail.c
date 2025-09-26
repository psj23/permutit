#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MSDOS) || defined(_MSDOS) || defined(__DOS__) || defined(__MSDOS__)
#define WINDOS 1
#elif defined(_WIN64) || defined(_WIN32) || defined(__WIN32__)
#define WINDOS 1
#elif defined(_POSIX_VERSION) || defined(__linux__) || defined(__unix__)
#define POSIXY 1
#endif

int cltail(int, char *const[], const char *, char ***, char ***);
static int dospar(int, char *const[], const char *, int *);
static int optdos(int, char *const[], const char *, char ***);
static int optget(int, char *const[], const char *);
static int clihlp(char *const[], const char *, const char *);
static void setdlm(int, char *const[], const char *);

char *DLMmsg = "Delimiter missing option:";
char *OMAmsg = "Option missing argument:";
char *OPTmsg = "Option invalid or missing argument:";
char *MALmsg = "calloc()";

char *optarg;
int opterr = 0;
int optind = 1;
int optopt;

#ifdef WINDOS
const char *nfostr = "?h";
#else
const char *nfostr = "?h";
#endif

int chkhlp = 1;
int errmsg = 1;
#ifdef POSIXY
int optdlm = '-';
#else
int optdlm = '\0';
#endif

#define ERR(_s, _c) if (opterr != 0 && *optstring != ':')\
 fprintf(stderr, "%s %c\n", _s, _c);
#define _ERRCH(_s, _c) if (opterr == 0 && errmsg != 0)\
 fprintf(stderr, "%s %c\n", _s, _c);
#define _ERRST(_p, _q) if (opterr == 0 && errmsg != 0)\
 fprintf(stderr, "%s %s\n", _p, _q);

int cltail(int argc, char *const argv[], const char *optstr,
 char ***opts, char ***args)
{
	size_t slen;
	char **u, **v;
	int c, i, nargs, nopts;

	if (optdlm != '-' && optdlm != '/') setdlm(argc, argv, optstr);
	if (chkhlp != 0) {
		c = clihlp(argv, optstr, nfostr);
		if (c != 0) return '?';
	}
	nopts = nargs = 0;
	while (argv[optind] != NULL) {
		i = optind;
		c = optget(argc, argv, optstr);
		if (c == -1) {
			if (*argv[i] != optdlm) {
				if (*optstr == '+') {
					nargs += argc - optind;
					break;
				}
				++nargs;
				optind += 1;
			}
			else if (argv[i][1] == '\0') {
				_ERRCH(DLMmsg, optdlm);
				return -1;
			}
			else {
				nargs += argc - optind;
				break;
			}
		}
		else if (c == ':') {
			_ERRCH(OMAmsg, optopt);
			return ':';
		}
		else if (c == '?') {
			_ERRCH(OPTmsg, optopt);
			return optopt;
		}
		else {
			++nopts;
			if (optdlm == '/' && argv[i][2] != '\0') {
				if (optarg == NULL) {
					c = dospar(argc, argv, optstr, &nopts);
					if (c != 0) return c;
				}
			}
		}
	}
	*opts = *args = NULL;
	*opts = calloc(nopts + 1, sizeof (char *));
	if (*opts == NULL) {
		perror(MALmsg);
		return ENOMEM;
	}
	*args = calloc(nargs + 1, sizeof (char *));
	if (*args == NULL) {
		perror(MALmsg);
		return ENOMEM;
	}
	u = *opts; v = *args;
	optind = 1;
	while (argv[optind] != NULL) {
		i = optind;
		c = optget(argc, argv, optstr);
		if (c == -1) {
			if (*argv[i] != optdlm) {
				if (*optstr == '+') {
					while (argv[i] != NULL) {
						*v = argv[i];
						++v; ++i;
					}
					break;
				}
				else {
					*v = argv[i]; ++v;
					optind += 1;
				}
			}
			else {
				while (argv[optind] != NULL) {
					*v = argv[optind];
					++v; optind += 1;
				}
				break;
			}
		}
		else {
			(optarg != NULL) ? (slen = strlen(optarg)) : (slen = 0);
			*u = calloc(slen + 2, sizeof (char));
			if (*u == NULL) {
				perror(MALmsg);
				return ENOMEM;
			}
			**u = c;
			*(*u + 1) = '\0';
			if (optarg != NULL)
				strcat(*u, optarg);
			else if (optdlm == '/' && i == optind) {
				++u;
				optdos(argc, argv, optstr, &u);
			}
			++u;
		}
	}
	*u = *v = NULL;
	return 0;
}

static int optget(int argc, char *const argv[], const char *optstring)
{
	char *ch_ptr;
	int c;
	static int strind = 1;

	if (strind == 1) {
		if (argv[optind] == NULL ||
		 *argv[optind] != optdlm || argv[optind][1] == '\0')
			return -1;
		if (argv[optind][1] == optdlm) {
			++optind;
			return -1;
		}
	}
	if (*optstring == '+') ++optstring;
	c = optopt = argv[optind][strind];
	ch_ptr = strchr(optstring, c);
	if (c != '+' && c != ':' && ch_ptr != NULL) {
		++ch_ptr;
		if (*ch_ptr == ':') {
			if (argv[optind][strind + 1] != '\0') {
				optarg = &argv[optind][strind + 1];
				++optind;
			}
			else if (optind + 1 >= argc) {
				ERR(OMAmsg, c);
				strind = 1;
				++optind;
				if (*optstring != ':')
					return '?';
				else
					return ':';
			}
			else {
				optarg = argv[optind + 1];
				optind += 2;
			}
			strind = 1;
		}
		else {
			if (argv[optind][strind + 1] == '\0') {
				strind = 1;
				++optind;
			}
			else
				++strind;
			optarg = NULL;
		}
		return c;
	}
	else {
		ERR("Invalid option:", c);
		++strind;
		if (argv[optind][strind] == '\0') {
			strind = 1;
			++optind;
		}
		return '?';
	}
}

static void setdlm(int argc, char *const argv[], const char *optstr)
{
	int i;

	if (optdlm != '-' && optdlm != '/') {
		optdlm = '-';
		for (i = 1; i < argc; ++i) {
			if (*argv[i] != '-' && *argv[i] != '/') {
				if (*optstr == '+') return;
				continue;
			}
			optdlm = *argv[i];
			if (*argv[i] == argv[i][1]) return;
			break;
		}
	}
	return;
}

static int clihlp(char *const argv[], const char *optstr, const char *hlpstr)
{
	char *ch_ptr, *p;
	int i, j;

	if (hlpstr == NULL || *hlpstr == '\0') return 0;
	i = 1;
	while (argv[i] != NULL) {
		p = argv[i];
		if (*p != optdlm) {
			if (*optstr == '+')
				return 0;
			else {
				++i; continue;
			}
		}
		j = 1; ++p;
		while (*p != '\0') {
			if (*p == optdlm && *(p + 1) == '\0') return 0;
			if (j % 2 == 0 && optdlm == '/') {
				if (*p != '/') return 0;
			}
			else if (*p != '+' && *p != ':') {
				ch_ptr = strchr(optstr, *p);
				if (ch_ptr != NULL) {
					if (*(ch_ptr + 1) == ':') {
						if (*(p + 1) == '\0') ++i;
						break;
					}
				}
				else {
					ch_ptr = strchr(hlpstr, *p);
					if (ch_ptr != NULL) return *p;
				}
			}
			++j; ++p;
		}
		++i;
	}
	return 0;
}

static int dospar(int argc, char *const argv[], const char *optstr, int *pnopts)
{
	int c, i;

	i = optind;
	do {
		c = optget(argc, argv, optstr);
		if (optopt != '/') {
			_ERRST(OPTmsg, argv[i]);
			return -1;
		}
		c = optget(argc, argv, optstr);
		if (c == ':') {
			_ERRCH(OMAmsg, optopt);
			return ':';
		}
		if (c == '?') {
			_ERRCH(OPTmsg, optopt);
			return optopt;
		}
		++*pnopts;
	} while (i == optind);
	return 0;
}

static int optdos(int argc, char *const argv[], const char *optstr,
 char ***optr)
{
	size_t y;
	int c, i;

	i = optind;
	do {
		c = optget(argc, argv, optstr);
		c = optget(argc, argv, optstr);
		(optarg != NULL) ? (y = strlen(optarg) + 2) : (y = 2);
		**optr = calloc(y + 2, sizeof (***optr));
		if (**optr == NULL) {
			fprintf(stderr, "%s\n", MALmsg);
			return ENOMEM;
		}
		***optr = c;
		*(**optr + 1) = '\0';
		if (optarg != NULL) strcat(**optr, optarg);
		++*optr;
	} while (i == optind);
	return 0;
}

