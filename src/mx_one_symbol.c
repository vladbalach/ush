#include "ush.h"

static void del_one_symbol(char **str, int *count, int position) {
    int i = position;
    unsigned char check = (unsigned char)(*str)[(*count) - position - 2];

    if ((*count) > 1 && (*count) > position + 1) {
        while ((check = check >> 6) == 2) {
            (*count)--;
            while (i > 0) {
                (*str)[(*count) - i - 1] = (*str)[(*count) - i];
                i--;
            }
            check = (unsigned char)(*str)[(*count) - position - 2];
            i = position;
        }
        (*count)--;
        while (i > 0) {
            (*str)[(*count) - i - 1] = (*str)[(*count) - i];
            i--;
        }
        (*str)[(*count) - 1] = 0;
    }
}

void mx_one_symbol(char **str, char ch, int *count, int position) {
    int i = 0;

    if (ch == 127)
        del_one_symbol(str, count, position);
    else {
        (*count)++;
        *str = realloc(*str, *count);
        while (i <= position) {
            (*str)[(*count) - i - 1] = (*str)[(*count) - i - 2];
            i++;
        }
        (*str)[(*count) - position - 2] = ch;
    }
}
