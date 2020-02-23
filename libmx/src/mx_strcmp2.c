#include "libmx.h"

int mx_strcmp2(const char *s1, const char *s2) {
    for (int i = 0; s2[i]; i++) {
        if (s1[i] < s2[i] || s1[i] > s2[i])
            return s1[i] - s2[i];
    }
    return 0;
}
