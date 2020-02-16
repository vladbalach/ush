#include "ush.h"

#define MX_X0 "x00" // \0
#define MX_XA "x07" // \a
#define MX_XB "x08" // \b
#define MX_XT "x09" // \t
#define MX_XN "x0a" // \n
#define MX_XV "x0b" // \v
#define MX_XF "x0c" // \f
#define MX_XR "x0d" // \r
#define MX_XE "x1b" // \e
#define MX_N0 "000" // \0
#define MX_NA "007" // \a
#define MX_NB "010" // \b
#define MX_NT "011" // \t
#define MX_NN "012" // \n
#define MX_NV "013" // \v
#define MX_NF "014" // \f
#define MX_NR "015" // \r
#define MX_NE "033" // \e

static bool control(int i, char *str, char *con) {
    if (str[i] == '\\' && strcmp(str + i + 1, con) == str[i + 4]) {
        return true;
    }
    return false;
}

static int print_echo_e(char *str) {
    char buf;
    int flag = 1;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i + 1]!= '\0' && str[i + 2] != '\0' && str[i + 3] != '\0') {
            flag = 0;
            if (control(i, str, "000") && (i += 3) > 0)
                buf = '\0';
            else if (control(i, str, "007") && (i += 3) > 0)
                buf = '\a';
            else if (control(i, str, "010") && (i += 3) > 0)
                buf = '\b';
            else if (control(i, str, "011") && (i += 3) > 0)
                buf = '\t';
            else if (control(i, str, "012") && (i += 3)> 0)
                buf = '\n';
            else if (control(i, str, "013") && (i += 3) > 0)
                buf = '\v';
            else if (control(i, str, "014") && (i += 3) > 0)
                buf = '\f';
            else if (control(i, str, "015") && (i += 3) > 0)
                buf = '\r';
            else if (control(i, str, "033") && (i += 3) > 0)
                buf = '\033';
            else if (control(i, str, "x00") && (i += 3) > 0)
                buf = '\0';
            else if (control(i, str, "x07") && (i += 3) > 0)
                buf = '\a';
            else if (control(i, str, "x08") && (i += 3) > 0)
                buf = '\b';
            else if (control(i, str, "x09") && (i += 3) > 0)
                buf = '\t';
            else if (control(i, str, "x0a") && (i += 3) > 0)
                buf = '\n';
            else if (control(i, str, "x0b") && (i += 3) > 0)
                buf = '\v';
            else if (control(i, str, "x0c") && (i += 3) > 0)
                buf = '\f';
            else if (control(i, str, "x0d") && (i += 3) > 0)
                buf = '\r';
            else if (control(i, str, "x1b") && (i += 3) > 0)
                buf = '\033';
            else
                flag = 1;
        }
        if (flag) {
            if (str[i] == '\\' && str[i + 1] == '\\' && ++i > 0)
                buf = '\\';
            else if (str[i] == '\\' && str[i + 1] == '0' && ++i > 0)
                buf = '\0';
            else if (str[i] == '\\' && str[i + 1] == 'a' && ++i > 0)
                buf = '\a';
            else if (str[i] == '\\' && str[i + 1] == 'b' && ++i > 0)
                buf = '\b';
            else if (str[i] == '\\' && str[i + 1] == 't' && ++i > 0)
                buf = '\t';
            else if (str[i] == '\\' && str[i + 1] == 'n' && ++i > 0)
                buf = '\n';
            else if (str[i] == '\\' && str[i + 1] == 'v' && ++i > 0)
                buf = '\v';
            else if (str[i] == '\\' && str[i + 1] == 'f' && ++i > 0)
                buf = '\f';
            else if (str[i] == '\\' && str[i + 1] == 'r' && ++i > 0)
                buf = '\r';
            else if (str[i] == '\\' && str[i + 1] == 'e' && ++i > 0)
                buf = '\033';
            else if (str[i] == '\\' && str[i + 1] == 'c' && ++i > 0)
                return 0;
            else
                buf = str[i];
        }
        flag = 1;
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

void mx_echo(char **str) {
    int i = 0;
    char *flags = checkflags(str, &i);
    int error = 1;

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
        for (i = i + 1; str[i]; i++) {
            error = print_echo_e(str[i]);
            if (str[i + 1] && error)
                write(1, " ", 1);
        }
        if (flags[0] != 'n' && error)
            write(1, "\n", 1);
    }
    free(flags);
}





