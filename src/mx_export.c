#include "ush.h"

void mx_export(const char *str, char **envp) {
    char *parametr = mx_strdup(str);
    putenv(parametr);
    int i = 0;
    while (envp[i]) {
        i++;
    }
    envp[i] = parametr;
    envp[i + 1] = 0;
}
