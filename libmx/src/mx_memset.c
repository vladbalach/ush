#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    char *p = (char *)b;

    while(len-- > 0)
        *p++ = c;
    return b;
}
