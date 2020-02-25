#include "libmx.h"

bool mx_is_str_starts(char *string, char *start) {
    if (mx_strncmp(string, start, mx_strlen(start)) == 0)
        return true;
    return false;
}
