#include "ush.h"

void mx_exec_token(t_token* token, int *fds, char pipe_status) {
   
    pid_t pid = fork();

        if (pid == -1) {        // error
            mx_printerr("fork error: ");
            mx_printerr(strerror(errno));
            return;
        }
        else if (pid == 0) {    // child
            mx_execute_proces(token);
        }
        else {                  // parent
            printf("START!\n");
            wait(0);
            printf("DONE!\n");
        }
}