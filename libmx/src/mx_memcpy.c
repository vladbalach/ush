#include "libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src,
                size_t n) {
    char *p = (char *)dst;
    char *d = (char *)src;

    for (size_t i = 0; i < n; i++)
        p[i] = d[i];
    return dst;
}
