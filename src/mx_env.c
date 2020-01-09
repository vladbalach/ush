#include "ush.h"

void mx_env(char *envp[]) {
    int i = 0;

    while (envp[i]) {
        mx_printstr(envp[i]);
        write(1,"\n",1);
        i++;
    }
}
