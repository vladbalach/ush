#include "libmx.h"

char *mx_strdup(const char *s1) {
    return mx_strcpy(mx_strnew(mx_strlen(s1)), s1);
}
