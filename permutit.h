#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MSDOS) || defined(_MSDOS) || defined(__DOS__) || defined(__MSDOS__)
#define MAXN ((size_t) 8)
#else
#define MAXN ((size_t) 12)
#endif

#define BUFLEN 128

