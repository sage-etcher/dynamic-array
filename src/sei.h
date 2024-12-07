
#ifndef SEI_HEADER
#define SEI_HEADER

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define clz(x) __builtin_clz(x)
#define popcount(x) __builtin_popcount(x)
#define popcountll(x) __builtin_popcountll(x)
#define PRECISION(x) popcount (x)
#define PRECISIONLL(x) popcountll (x)

#define SEI_LSHFT_U(max, a) (a >= PRECISION (max))
#define SEI_LSHFT_ULL(max, a) (a >= PRECISIONLL (max))
#define SEI_LSHFT_SIZE(a) SEI_LSHFT_ULL (SIZE_MAX, a)

#define SEI_SUB_U(a, b) ((a) < (b))
#define SEI_SUB_SIZE(a, b) SEI_SUB_U (a, b)

#define SEI_ADD_U(max, a, b) ((max) - (a) < (b))
#define SEI_ADD_SIZE(a, b) SEI_ADD_U (SIZE_MAX, a, b)

#define SEI_MULT_U(max, a, b) ((max) / (a) < (b))
#define SEI_MULT_SIZE(a, b) SEI_MULT_U (SIZE_MAX, a, b)
#define SEI_ALLOC_ARR(t, n) SEI_MULT_SIZE (sizeof (t), n)

#endif // !SEI_HEADER
/* end of file */
