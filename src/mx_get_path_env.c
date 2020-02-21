#include "ush.h"

char *mx_get_path_env(char *str1, char *str2, int *i) {
    char *result = NULL;

   if ((mx_strlen(str1) > 2 && str1[1] == 'P') 
        || (mx_strlen(str1) > 3 && str1[1] == 'i')) {
        str1++;
        if (*str1 == 'i')
            for (; *str1 == 'i'; str1++);
        str1++;
        result = mx_strdup(str1);
    }
    else {
        (*i)++;
        if (str2)
            result = mx_strdup(str2);
    }
    return result;
}

