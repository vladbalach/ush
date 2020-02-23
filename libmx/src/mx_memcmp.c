#include "libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    char *a1;
    char *a2;
    int a = 0;

    if (n == 0)
        return 0;
    a1 = (char *)s1;
    a2 = (char *)s2;
    for (size_t i = 0; i < n; i++) {
        if ((a1[i] - a2[i]) != 0) {
            return a1[i] - a2[i];
        }
    }
    return a;
}
