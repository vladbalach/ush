#include "ush.h"

int mx_0_and_x(char *str, int *i) {
    char *buf = mx_strnew(3);
    char *end = NULL;
    int result = 0;

    if (str[*i + 1] == 'x' && mx_reg(str + *i + 2, "^[0-9a-fA-F].*$")) {
        *i += 2;
        buf[0] = str[*i];
        buf[1] = str[(*i) + 1];
        (*i)++;
        result = (int)strtol(buf, &end, 16);
    }
    else if (str[*i + 1] == '0' && mx_reg(str + *i + 2, "^[0-7]{2,3}.*$")) {
        *i += 2;
        buf[0] = str[*i];
        buf[1] = str[(*i) + 1];
        if (str[*i + 2] >= '0' && str[*i + 2] <= '7') {
            buf[2] = str[*i + 2];
            (*i)++;
        }
        (*i)++;
        result = (int)strtol(buf, &end, 8);
    }
    free(buf);
    return result;
}
