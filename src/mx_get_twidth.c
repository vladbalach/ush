#include "ush.h"

int mx_get_twidth() {
    char *termtype = getenv("TERM");
    char *buff = malloc(2048);
    int success = 0;
    int col = 0;
    bool if_term = 0;

    if (termtype == 0) {
        termtype = mx_strdup("xterm-256color");
        if_term = 1;
    }
    if ((success = tgetent (buff, termtype)) < 0) {
        fprintf(stderr, "ush: Could not access the termcap data base.\n");
        exit(1);
    }
    free(buff);
    if (if_term)
        mx_strdel(&termtype);
    col = tgetnum("co");
    return col;
}