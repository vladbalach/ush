#include "libmx.h"

char *mx_strjoin3(char *s1, char *s2) {
    char *newStr = NULL;

    newStr = mx_strjoin(s1,s2);
    free(s1);
    free(s2);
    return newStr;
}
