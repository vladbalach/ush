#include "libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little,
                size_t little_len) {
    char *s1 = (char *)big;
    char *s2 = (char *)little;
    size_t j;

    for(size_t i = 0; i < big_len; i++){
        j = 0;
        if(s1[i] == s2[j]) {
            while (s1[i] == s2[j] && i < big_len && j < little_len) {
                i++;
                j++;
                if (j == little_len)
                    return &s1[i - j];
            }
        }
        i -= j;
    }
    return 0;
}
