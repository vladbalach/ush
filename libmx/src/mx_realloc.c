#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    char *s = (char *) ptr;
    void *d = NULL;
    size_t i;

    if (s == 0 && size > 0)
        return malloc(size);
    if (s != 0 && size == 0){
        free(s);
        return 0;
    }
    d = malloc(size);
    i = mx_strlen(s);
    if (i < size)
        size = i + 1;
    mx_memcpy(d, s, size);
    free(s);
    return d;
}
