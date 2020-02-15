#include "ush.h"

void mx_do_replace(char **str, size_t start, size_t end, char *str_new) {
    char *newStr = NULL;;
    int i = mx_strlen(*str);
    int sum = 0;

    if (str_new)
        sum = mx_strlen(str_new);
    newStr = (char*) malloc (i - (end - start) + sum + 1);
    if (newStr) {
        for (i = 0; i < start; i++)
            newStr[i] = (*str)[i];
        while (sum != 0 && str_new[i - start]) {
            newStr[i] = str_new[i - start];
            i++;
        }
        while((*str)[end])
            newStr[i++] = (*str)[end++];
        newStr[i] = 0;
    }
    free(*str);
    *str = newStr;
}
