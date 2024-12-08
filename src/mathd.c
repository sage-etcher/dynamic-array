
#include "mathd.h"

#include <errno.h>
#include "sei.h"
#include <stddef.h>


size_t
log2s (size_t x)
{
    size_t result = sizeof (size_t) * CHAR_BIT - clzsz (x) - 1;
    return result;
}


size_t
pow2s (size_t x)
{
    if (SEI_LSHFT_SIZE (x))
    {
        errno = ERANGE;
        return 0;
    }
    size_t result = (size_t)1 << x;
    return result;
}


/* end of file */
