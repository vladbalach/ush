#include "ush.h"

int mx_get_twidth() {
    char termtype[] = "xterm-256color";
    char *buff = malloc(2048);
    int success = 0;
    int col = 0;

    if ((success = tgetent (buff, termtype)) < 0) {
        fprintf(stderr, "ush: Could not access the termcap data base.\n");
        exit(1);
    }
    free(buff);
    col = tgetnum("co");
    return col;
}
