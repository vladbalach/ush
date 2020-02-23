#include "libmx.h"

char *mx_arrstr_to_str(char **strs) {
    char *new_string = 0;
    int i = -1;

    while (strs[++i]) {
        new_string = mx_strjoin2(new_string, strs[i]);
        if (strs[i + 1])
            new_string = mx_strjoin2(new_string, " ");
    }
    return new_string;
}
