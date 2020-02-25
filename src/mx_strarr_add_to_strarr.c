#include "ush.h"

static int len_strarr(char **str) {
    int count = 0;

    while (str[count]) 
        count++;
    return count;
}

void mx_strarr_add_to_strarr(char ***strs, char ***str) {
    int count = 0;
    int count2 = 0;

    if ((strs == 0) || (str == 0))
        return;
    if (*strs == 0) {
        *strs = *str;
        return;
    }
    count = len_strarr(*strs);
    count2 = len_strarr(*str);
    *strs = (char **) realloc(*strs, sizeof(char *) * (count  + count2 + 1));
    count2 = 0;
    while ((*str)[count2]) {
        (*strs)[count++] = (*str)[count2++];
    }
    (*strs)[count] = 0;
    free(*str);
    return;
}
