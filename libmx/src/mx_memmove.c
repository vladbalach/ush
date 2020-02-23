#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    char *s1 = (char *)dst;
    char *s2 = (char *)src;
    char a[len];

    for (size_t i = 0; i < len; i++)
        a[i] = s2[i];
    for (size_t i = 0; i < len; i++)
        s1[i] = a[i];
    return dst;
}
