#include "ush.h"

void mx_subs(char **str) {
    for (int i = 0; str[0] && str[0][i]; i++)
        if (str[0][i] == '\x0d')
            str[0][i] = '\n';
}
