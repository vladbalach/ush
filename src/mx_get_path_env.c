#include "ush.h"

char *mx_get_path_env(char *str1, char *str2, int *i) {
    char *result = NULL;

    if (mx_strlen(str1) > 2) {
        str1 += 2;
        result = mx_strdup(str1);
    }
    else {
        (*i)++;
        if (str2)
            result = mx_strdup(str2);
    }
    return result;
}
