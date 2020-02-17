#include "libmx.h"

char *mx_strdup(const char *s1) {
    if (s1 == 0)
        return 0;
    return mx_strcpy(mx_strnew(mx_strlen(s1)), s1);
}
