#include "libmx.h"

void mx_print_strarr(char **arr, const char *delim) {
    int b = 0;

    if (arr != NULL && delim != NULL) {
        while (arr[b] != NULL)
            b++; 
        for (int i = 0; arr[i]; i++){
            mx_printstr(arr[i]);
            if (i != (b - 1)) 
                mx_printstr(delim);
        }
        mx_printchar('\n');
    }
}
