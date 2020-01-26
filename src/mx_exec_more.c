#include "ush.h"

void mx_exec_more(t_tnode *root, int *fds, int operatorStatus) {
    int fdsBuff[2];
   if ((operatorStatus & 3) == 0) {
        int fd = open(((t_token*)(root->right->data))->value[0], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
        fdsBuff[0] = 0;
        fdsBuff[1] = fd;
        mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE);
    }
   if (operatorStatus & OP_PIPE_W) {
       int fd = open(((t_token*)(root->right->data))->value[0], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
       fdsBuff[0] = 0;
       fdsBuff[1] = fd;
       mx_execute_tree(root->left, fdsBuff, operatorStatus | OP_MORE);
       close(fd);
       fd = open(((t_token*)(root->right->data))->value[0], O_RDWR , S_IWRITE | S_IREAD);
       fdsBuff[0] = fd;
       fdsBuff[1] = fds[1];
        mx_execute_tree(root->parent, fdsBuff, operatorStatus | OP_MORE | LEFT_VISITED);
       close(fd);

   }
}
