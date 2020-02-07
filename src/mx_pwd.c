#include "ush.h"


static int get_flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    while(str[++i]) {
        if (str[i] == 'L') 
            flags |= 1;
        else if (str[i] == 'P') 
            flags |= 2;
        else {
            return -1;
        }
    }
    return flags;
}
/* 
* 1-st bit -> -L
* 2-nd bit -> -P
*/
static int get_flags(char **argv, int *i) {
    int flags = 0;

    while(argv[++(*i)]) {
        if (argv[(*i)][0] != '-')
            return flags;
        int curr = get_flags_from_line(argv[*i]);
        if (curr == -1) {
            return 0;
        }
        flags |= curr;
    }
    i--;
    return flags;
}

int mx_pwd(char **argv, t_info *info) {
    int i = 0;
    int flags = get_flags(argv, &i);

    if (flags & 2) {
        printf("%s\n", info->pwdP);
    }
    else
        printf("%s\n", info->pwdL);
    return 0;
}
