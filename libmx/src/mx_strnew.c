#include "libmx.h"

char *mx_strnew(const int size) {
    char *s = NULL;

    if (size < 0) 
        return NULL;
    s = (char *)malloc(size + 1);
    if (s != 0) 
        for(int i = 0; i <= size; i++)
            s[i]= '\0';
    return s;
}
