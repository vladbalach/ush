#include "ush.h"

// static int syntax_analiz(char *temp, int *position, int end) {

// }

void mx_parsing(char *str) {
    int end = mx_strlen(str);
    char *temp = mx_parsing_input(str);
    char **comands = 0;

    if (temp == 0) {
        return;
    }
    comands = mx_create_comands(temp, end);
    // mx_printstr("\n");
    for(int i = 0;  comands[i]; i++) {
        mx_printstr(comands[i]);
        mx_printstr("\n");
    }
}
