#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (str == 0 || sub == 0)
        return - 2;
    if (mx_strstr(str, sub) == 0)
        return - 1;
    return mx_strlen(str) - mx_strlen(mx_strstr(str, sub));
}
