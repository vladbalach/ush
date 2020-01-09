#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    int a = 0;
    int b = 0;

    if (str == 0 || sub == 0)
        return - 1;
    while (a < mx_strlen(str)) {
        if (mx_strcmp2(&str[a], &sub[0]) == 0)
            for (int i = 0; sub[i]; i++, a++) {
                if (mx_strcmp2(&str[a], &sub[i]) != 0) {
                    a = a - i;
                    break;
                }
                if (sub[i + 1] == 0)
                    b++;
            }
        else
            a++;
    }
    return b;
}
