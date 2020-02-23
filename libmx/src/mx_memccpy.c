#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c,
                 size_t n) {
    char *p = (char *)dst;
    char *d = (char *)src;
    size_t i = 0;

    for (; i < n; i++) {
        p[i] = d[i];
        if(p[i] == c)
            break;
    }
    if (i == n)
        return 0;
    return &p[i + 1];        
}
