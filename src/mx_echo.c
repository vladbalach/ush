#include "ush.h"

static int print_echo_d(char *str, int *i) {
    if (str[0] == '\\' && str[1] == 'a' && ++(*i) > 0)
        return '\a';
    else if (str[0] == '\\' && str[1] == 'b' && ++(*i) > 0)
        return '\b';
    else if (str[0] == '\\' && str[1] == 't' && ++(*i) > 0)
        return '\t';
    else if (str[0] == '\\' && str[1] == 'n' && ++(*i) > 0)
        return '\n';
    else if (str[0] == '\\' && str[1] == 'v' && ++(*i) > 0)
        return '\v';
    else if (str[0] == '\\' && str[1] == 'f' && ++(*i) > 0)
        return '\f';
    else if (str[0] == '\\' && str[1] == 'r' && ++(*i) > 0)
        return '\r';
    else if (str[0] == '\\' && str[1] == 'e' && ++(*i) > 0)
        return '\033';
    return -1;
}

static int print_echo_e(char *str) {
    char buf;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\' 
            && mx_reg(str + i + 1, "^(x[0-9a-fA-F]{2}.*)|(0[0-7]{2,3}.*)$")) {
            buf = mx_0_and_x(str, &i);
        }
        else if(str[i] == '\\' && str[i + 1] == '\\')
            buf = '\\';
        else if (str[i] == '\\' && str[i + 1] == '0' && ++i > 0)
            buf = '\0';
        else if ((buf = print_echo_d(&str[i], &i)) != -1);
        else if (str[i] == '\\' && str[i + 1] == 'c' && ++i > 0)
            return 0;
        else
            buf = str[i];
        write(1, &buf, 1);
    }
    return 1;
}

static char *checkflags(char **str, int *counter) {
    char *flags = mx_strnew(2);

    flags[0] = '0';
    flags[1] = 'e';
    for (int i = 1; str[i]; i++) {
        if (str[i][0] != '-')
            return flags;
        for (int n = 1; str[i][n] != '\0'; n++)
            if (str[i][n] != 'n' && str[i][n] != 'e' && str[i][n] != 'E')
                return flags;
        *counter = i;
        for (int n = 1; str[i][n]; n++) {
            if (str[i][n] == 'n')
                flags[0] = 'n';
            else
                flags[1] = str[i][n] == 'e' ? 'e' : 'E';
        }
    }
    return flags;
}

static void print_e(int i, char *flags, char **str) {
    int error = 1;    

    for (i = i + 1; str[i]; i++) {
        error = print_echo_e(str[i]);
        if (str[i + 1] && error)
            write(1, " ", 1);
    }
    if (flags[0] != 'n' && error)
        write(1, "\n", 1);
}

void mx_echo(char **str, t_info *info) {
    int i = 0;
    char *flags = checkflags(str, &i);

    if (flags[1] == 'E') {
        for (i = i + 1; str[i]; i++) {
            write(1, str[i], mx_strlen(str[i]));
            if (str[i + 1])
                write(1, " ", 1);
        }
        if (flags[0] != 'n')
            write(1, "\n", 1);
    }
    else {
        print_e(i, flags, str);
    }
    info->last_status = 0;
    free(flags);
}





