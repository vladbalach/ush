#include "ush.h"

void mx_do_replace(char **str, size_t start, size_t end, char *str_new) {
    char *new_str = NULL;;
    size_t i = mx_strlen(*str);
    int sum = 0;

    if (str_new)
        sum = mx_strlen(str_new);
    new_str = (char*) malloc (i - (end - start) + sum + 1);
    if (new_str) {
        for (i = 0; i < start; i++)
            new_str[i] = (*str)[i];
        while (sum != 0 && str_new[i - start]) {
            new_str[i] = str_new[i - start];
            i++;
        }
        while((*str)[end])
            new_str[i++] = (*str)[end++];
        new_str[i] = 0;
    }
    free(*str);
    *str = new_str;
}
