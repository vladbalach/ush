#include "libmx.h"

char *mx_strncpy(char *dst, const char *src, int len) {
    int i = 0;

    if (dst != 0) {
        for (; i < len && i < mx_strlen(src); i++) {
            dst[i] = src[i];
        }
        if (mx_strlen(src) < len)
            for (; i < len; i++)
                dst[i] = '\0';
    }
    return dst;
}
