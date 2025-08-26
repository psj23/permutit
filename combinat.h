#if defined(MSDOS) || defined(_MSDOS) || defined(__DOS__) || defined(__MSDOS__)
#define MAXN (size_t) 8 
#else
#define MAXN (size_t) 12
#endif

void combnext(size_t *, size_t, size_t);
void permnext(size_t *, size_t, size_t);
