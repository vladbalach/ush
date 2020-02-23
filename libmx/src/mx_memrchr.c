#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    char *s1 = (char *)s;

    for (int i = n - 1; i >= 0 && i < (int)n ; i--)
        if (s1[i] == c)
            return &s1[i];
    return 0;
}
