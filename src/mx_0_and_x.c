#include "ush.h"

static int get_hex_and_oct(char *buf, char *str, int *i) {
    if (str[*i + 1] == 'x' && mx_reg(str + *i + 2, "^[0-9a-fA-F].*$")) {
        *i += 2;
        buf[0] = str[*i];
        buf[1] = str[(*i) + 1];
        (*i)++;
        return (int)strtol(buf, NULL, 16);
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
        return (int)strtol(buf, NULL, 8);
    }
    return -1;
}

int mx_0_and_x(char *str, int *i) {
    char *buf = mx_strnew(3);
    int result = 0;

    result = get_hex_and_oct(buf, str, i);
    free(buf);
    return result;
}

