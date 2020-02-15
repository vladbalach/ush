#include "libmx.h"

void mx_del_strarr(char ***arr) {
    char **d;
    
    if (arr && *arr) {
        d = *arr;
        for (int i = 0 ; d[i]; i++) {
            mx_strdel(&d[i]);
        }
        free(*arr);
        *arr = NULL;
    }
} 
