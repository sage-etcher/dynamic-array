

#include "darr.h"


int
main (int argc, char **argv)
{
    int *darr = DARR_INIT (int, 0);
    if (!darr) goto cleanup;

    /* extend */
    void *temp = darr_push (darr, 10);
    if (!temp) goto cleanup;
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

    int result = 0;
    int *p_result = darr_pop (darr);
    if (!p_result) goto cleanup;
    result = *p_result;

    (void)darr_slide (darr, 4);

    size_t count = darr_count (darr);

cleanup:
    darr_free (darr); darr = NULL;
    return 0;
}


/* end of file */
