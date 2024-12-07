
#include "mathd.h"

#include <errno.h>
#include "sei.h"
#include <stddef.h>


size_t
log2s (size_t x)
{
    return sizeof (size_t) * CHAR_BIT - clz(x) - 1;
}


size_t
pow2s (size_t x)
{
    if (SEI_LSHFT_SIZE (x))
    {
        errno = ERANGE;
        return 0;
    }
    return 1 << x;
}


/* end of file */
