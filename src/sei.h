
#ifndef SEI_HEADER
#define SEI_HEADER

#include <limits.h>
#include <stddef.h>
#include <stdint.h>


#define clz(x) __builtin_clz(x)
#define popcount(x) __builtin_popcount(x)
#define PRECISION(x) popcount (x)

#define clzl(x) __builtin_clzl(x)
#define popcountl(x) __builtin_popcountl(x)
#define PRECISIONL(x) popcountl (x)

#define SEI_LSHFT_U(max, a) (a >= PRECISION (max))
#define SEI_LSHFT_UL(max, a) (a >= PRECISIONL (max))
#define SEI_LSHFT_SIZE(a) SEI_LSHFT_SZ (SIZE_MAX, a)

#define SEI_SUB_U(a, b) ((a) < (b))
#define SEI_SUB_SIZE(a, b) SEI_SUB_U (a, b)

#define SEI_ADD_U(max, a, b) ((max) - (a) < (b))
#define SEI_ADD_SIZE(a, b) SEI_ADD_U (SIZE_MAX, a, b)

#define SEI_MULT_U(max, a, b) ((max) / (a) < (b))
#define SEI_MULT_SIZE(a, b) SEI_MULT_U (SIZE_MAX, a, b)
#define SEI_ALLOC_ARR(t, n) SEI_MULT_SIZE (sizeof (t), n)

/* 32-bit vs 64-bit dependent */
#if defined(__x86_64) || defined (_M_AMD64)
#    define clzll(x) __builtin_clzll(x)
#    define popcountll(x) __builtin_popcountll(x)
#    define PRECISIONLL(x) popcountll (x)
#    define SEI_LSHFT_ULL(max, a) (a >= PRECISIONLL (max))

#    define clzsz clzll
#    define popcountsz popcountll
#    define PRECISIONSZ PRECISIONLL
#    define SEI_LSHFT_SZ SEI_LSHFT_ULL
#else
#    define clzsz clzl
#    define popcountsz popcountl
#    define PRECISIONSZ PRECISIONL
#    define SEI_LSHFT_SZ SEI_LSHFT_UL
#endif


#endif // !SEI_HEADER
/* end of file */
