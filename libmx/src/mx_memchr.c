#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    char *s1 = (char *)s;

    for (size_t i = 0; i < n; i++)
        if (s1[i] == c)
            return &s1[i];
    return 0;
}
