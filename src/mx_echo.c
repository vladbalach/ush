#include "ush.h"


void mx_echo(char **str) {
    int i = 0;

    while (str[i]) {
        mx_printstr(str[i]);
    }
    
}
