#include "libmx.h"

int mx_isspace(char c) {
    if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
        || c == '\v')
        return 1;
    else
        return 0;
}
