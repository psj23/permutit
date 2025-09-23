#include <stdlib.h>

void combnext(size_t *, size_t, size_t);
void permnext(size_t *, size_t, size_t);

void combnext(size_t *seq, size_t n, size_t r)
{
	size_t *outside, *p, *q;

	p = q = outside = seq + r;
	--p;
	while (q > seq) {
		++*p;
		if (*p > n - (outside - p)) {
			--p;
			--q;
		}
		else
			break;
	}
	if (q == seq) {
		p = seq;
		if (*p > n - r)
			*p = 0;
		++q;
	}
	while (q < outside) {
		*q = *p + 1;
		++p;
		++q;
	}
	return;
}

void permnext(size_t *seq, size_t n, size_t r)
{
	size_t *outarray, *pivot, *q1, *q2;
	size_t x;

	pivot = q1 = outarray = seq + r;
	--pivot;
	while (q1 > seq) {
		if (++*pivot == n) { --pivot; --q1; continue; }
		for (q2 = seq; q2 < pivot; ++q2) if (*q2 == *pivot) break;
		if (q2 == pivot) break;
	}
	while (q1 < outarray) {
		x = 0;
		while (x < n) {
			for (q2 = seq; q2 < q1; ++q2) if (*q2 == x) break;
			if (q2 == q1) { *q1 = x; break; }
			++x;
		}
		++q1;
	}
	return;
}
