#include "ush.h"

static bool print() {
    mx_printerr("\nusage: env [-i] [-P utilpath] [-u name]\n");
    mx_printerr("\t[name=value ...] [utility [argument ...]]\n");
    return 1;
}

bool mx_print_error_env(char *str, int flag) {
    char s;

    if (flag == 2) {
        mx_printerr("env: setenv =: Invalid argument\n");
        return 1;
    }
    if (str[1] == 'i')
        for (int i = 1; str[i] == 'i'; i++) 
            s = str[i + 1];
    else
        s = str[1];
    if (flag)
        mx_printerr("env: illegal option -- ");
    else {
        s = str[1];
        mx_printerr("env: option requires an argument -- ");
    }
    write(2, &s, 1);
    return print();
}

