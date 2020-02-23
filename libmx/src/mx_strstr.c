#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    int a = 0;

    while (haystack[a]) {
        for (int i = 0; needle[i]; i++, a++) {
            if (mx_strcmp2(&haystack[a], &needle[i]) != 0) {
                a = a - i;
                break;
            }
            if (needle[i + 1] == 0)
                return (char *)(&haystack[a - i]); 
        }
        a++;
    }
    return NULL;
}
