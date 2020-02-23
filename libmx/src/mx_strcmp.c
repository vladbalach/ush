#include "libmx.h"

int mx_strcmp(const char *s1, const char *s2) {
    unsigned char *s11 = (unsigned char *)s1;
    unsigned char *s22 = (unsigned char *)s2;

    for (int i = 0; s11[i] || s22[i]; i++) {
        if (s11[i] < s22[i] || s11[i] > s22[i])
            return s11[i] - s22[i];
    }
    return 0;
}
