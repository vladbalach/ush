#include "libmx.h"

char *mx_strndup(const char *s1, size_t n) {
    return mx_memcpy(mx_strnew(n), s1, n);
}
