

#ifndef DARR_HEADER

#include <stddef.h>

#define DARR_INIT(t, n) (t *)(darr_init (_Alignof (t), (n)))
void  *darr_init  (size_t type_size, size_t n);
void  *darr_push  (void *s_arr, size_t n);
void  *darr_pop   (void *s_arr);
void  *darr_slide (void *s_arr, size_t n);
size_t darr_count (void *s_arr);
void   darr_free  (void *s_arr);

#endif // !DARR_HEADER
/* end of file */
