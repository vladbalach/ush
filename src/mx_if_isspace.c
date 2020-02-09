#include "ush.h"

static char if_isspace2(char s) {
    char temp = s;

     if (s == 0)
        temp = '0';
    else if (s == '\a')
        temp = 'a';
    else if (s == '\b')
        temp = 'b';
    else if (s == '\t')
        temp = 't';
    else if (s == '\n')
        temp = 'n';
    else if (s == '\v')
        temp = 'v';
    else if (s == '\f')
        temp = 'f';
    else if (s == '\r')
        temp = 'r';
    return temp;
}

static char if_isspace3(char s) {
    char temp = s;

    if (s == '0')
        temp = 0;
    else if (s == 'a')
        temp = '\a';
    else if (s == 'b')
        temp = '\b';
    else if (s == 't')
        temp = '\t';
    else if (s == 'n')
        temp = '\n';
    else if (s == 'v')
        temp = '\v';
    else if (s == 'f')
        temp = '\f';
    else if (s == 'r')
        temp = '\r';
    return temp;
}

char mx_if_isspace(char s) {
    char temp = s;

    if (s < 15 && s > -1)
        temp = if_isspace2(s);
    else if (s == 48 || (s > 96 && s < 19))
        temp = if_isspace3(s);
    return temp;
}
