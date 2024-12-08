
#include "darr.h"

#include <errno.h>
#include "mathd.h"
#include "sei.h"
#include <stddef.h>
#include <stdlib.h>


#ifndef errno_t
    typedef int errno_t;
#endif


#define DARR_REMOVE_HEADER(ptr, field) \
    ((dynamic_array_t *)((uintptr_t)ptr - offsetof (dynamic_array_t, field)))
#define DARR_MAKE_HEADER(obj, field) (&(obj->field))


struct dynamic_array
{
    size_t count;
    size_t alloc;
    size_t type_size;
    uint8_t data[];
};
typedef struct dynamic_array dynamic_array_t;


static dynamic_array_t *darr_i_init (size_t type_size, size_t n);
static void darr_i_free (dynamic_array_t *s);
static size_t darr_i_calc_size (size_t type_size, size_t new_alloc);
static size_t darr_i_calc_alloc (size_t count);
static dynamic_array_t *darr_i_realloc (dynamic_array_t *s, size_t size);
static dynamic_array_t *darr_i_extend (dynamic_array_t *s);
static int darr_i_add (dynamic_array_t *s, size_t n);
static int darr_i_sub (dynamic_array_t *s, size_t n);


/* public api */
void *
darr_init (size_t type_size, size_t n)
{
    dynamic_array_t *s = darr_i_init (type_size, n);
    if (!s) return NULL;

    return DARR_MAKE_HEADER(s, data);
}


void *
darr_push (void *s_arr, size_t n)
{
    dynamic_array_t *s = NULL;

    if (!s_arr)
    {
        errno = EINVAL;
        return NULL;
    }

    s = DARR_REMOVE_HEADER (s_arr, data);
    if (darr_i_add (s, n)) return NULL;

    s = darr_i_extend (s);
    if (!s) return NULL;

    return DARR_MAKE_HEADER (s, data);
}


void *
darr_pop (void *s_arr)
{
    dynamic_array_t *s = NULL;

    if (!s_arr)
    {
        errno = EINVAL;
        return NULL;
    }

    s = DARR_REMOVE_HEADER (s_arr, data);
    if (darr_i_sub (s, 1)) return NULL;

    return (void *)(s->data + (s->count * s->type_size));
}


void *
darr_slide (void *s_arr, size_t n)
{
    dynamic_array_t *s = NULL;

    if (!s_arr)
    {
        errno = EINVAL;
        return NULL;
    }

    s = DARR_REMOVE_HEADER (s_arr, data);
    if (darr_i_sub (s, n)) return NULL;

    return DARR_MAKE_HEADER (s, data);
}


size_t
darr_count (void *s_arr)
{
    dynamic_array_t *s = NULL;

    if (!s_arr)
    {
        errno = EINVAL;
        return 0;
    }

    s = DARR_REMOVE_HEADER (s_arr, data);

    return s->count;
}


void
darr_free (void *s_arr)
{
    dynamic_array_t *s = NULL;

    if (!s_arr) return;

    s = DARR_REMOVE_HEADER (s_arr, data);

    darr_i_free (s);
}


/* private api */
static dynamic_array_t *
darr_i_init (size_t type_size, size_t n)
{
    dynamic_array_t *s = NULL;
    size_t initial_alloc = 0;
    size_t initial_size  = 0;

    if (type_size == 0)
    {
        errno = EINVAL;
        return NULL;
    }

    initial_alloc = darr_i_calc_alloc (n);
    if (initial_alloc == 0) return NULL;

    initial_size = darr_i_calc_size (type_size, initial_alloc);
    if (initial_size == 0) return NULL;

    s = darr_i_realloc (NULL, initial_size);
    if (!s) return NULL;

    s->type_size = type_size;
    s->alloc     = initial_alloc;
    s->count     = 0;

    return s;
}


static size_t
darr_i_calc_alloc (size_t count)
{
    size_t new_alloc = pow2s (log2s (count) + 1);
    return new_alloc;
}


static size_t
darr_i_calc_size (size_t type_size, size_t new_alloc)
{
    size_t new_size  = 0;

    if ((new_alloc == 0) || (type_size == 0))
    {
        errno = EINVAL;
        return 0;
    }

    if ((SEI_MULT_SIZE (type_size, new_alloc)) ||
        (SEI_ADD_SIZE (sizeof (dynamic_array_t), (type_size * new_alloc))))
    {
        errno = ERANGE;
        return 0;
    }
    new_size = type_size * new_alloc + sizeof (dynamic_array_t);

    return new_size;
}


static dynamic_array_t *
darr_i_realloc (dynamic_array_t *s, size_t size)
{
    if (size == 0)
    {
        errno = EINVAL;
        return NULL;
    }
    void *temp = realloc (s, size);
    if (!temp) return NULL;
    s = temp;

    return s;
}


static dynamic_array_t *
darr_i_extend (dynamic_array_t *s)
{
    size_t new_alloc = 0;
    size_t new_size  = 0;

    /* no null ptrs pls */
    if (!s)
    {
        errno = EINVAL;
        return NULL;
    }

    /* do we need to allocate more? */
    if (s->alloc > s->count) return s;

    /* allocate more */
    new_alloc = darr_i_calc_alloc (s->count);
    if (new_alloc == 0) return NULL;

    new_size = darr_i_calc_size (s->type_size, new_alloc);
    if (new_size == 0) return NULL;

    void *temp = darr_i_realloc (s, new_size);
    if (!temp) return NULL;

    s = temp;
    s->alloc = new_alloc;

    return s;
}


static int
darr_i_add (dynamic_array_t *s, size_t n)
{
    if (!s)
    {
        errno = EINVAL;
        return 1;
    }

    if (SEI_ADD_SIZE (n, s->count))
    {
        errno = ERANGE;
        return 1;
    }
    s->count += n;

    return 0;
}


static int
darr_i_sub (dynamic_array_t *s, size_t n)
{
    if (!s)
    {
        errno = EINVAL;
        return 1;
    }

    if (SEI_SUB_SIZE (s->count, n))
    {
        s->count = 0;
        errno = ERANGE;
        return 1;
    }
    s->count -= n;

    return 0;
}


static void
darr_i_free (dynamic_array_t *s)
{
    if (!s) return;

    s->count      = 0;
    s->alloc      = 0;
    s->type_size  = 0;
    free (s);

    return;

}


/* end of file */
