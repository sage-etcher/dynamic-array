
#include "darr.h"
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct tests {
    char m[2];
};

void
log_error (char *msg)
{
    fprintf (stderr, "error: %s: %d: %s\n", msg, errno, strerror (errno));
}


int
main (int argc, char **argv)
{
    const size_t half_overflow    = 9223372036854775808;
    const size_t quarter_overflow = 4611686018427387904;
    const size_t eighth_overflow  = 2305843009213693952;

    errno = 0;
    int *darr = DARR_INIT (int, 0);
    if (!darr)
    {
        log_error ("DARR_INIT()");
        goto cleanup;
    }

    /* extend */
    errno = 0;
    void *temp = darr_push (darr, 10);
    if (!temp)
    {
        log_error ("darr_push()");
        goto cleanup;
    }
    darr = temp;

    darr[0] = 0;
    darr[1] = 1;
    darr[2] = 2;
    darr[3] = 3;
    darr[4] = 4;
    darr[5] = 5;
    darr[6] = 6;
    darr[7] = 7;
    darr[8] = 8;
    darr[9] = 9;

    errno = 0;
    int result = 0;
    int *p_result = darr_pop (darr);
    if (!p_result)
    {
        log_error ("darr_pop()");
        goto cleanup;
    }
    result = *p_result;
    printf ("result: %d\n", result);

    errno = 0;
    if (!darr_slide (darr, 4))
    {
        log_error ("darr_slide()");
        goto cleanup;
    }

    size_t count = darr_count (darr);
    printf ("count:  %zu\n", count);

    for (size_t i = 0; i < count; i++)
    {
        printf ("darr[%zu] = %d\n", i, darr[i]);
    }

cleanup:
    darr_free (darr); darr = NULL;
    return 0;
}


/* end of file */
