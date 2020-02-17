#include "ush.h"

t_tnode *mx_create_tnode(void *data) {
    t_tnode *newNode = (t_tnode*) malloc (sizeof(t_tnode));

    newNode->data = data;
    newNode->left = 0;
    newNode->right = 0;
    newNode->parent = 0;
    return newNode;
}
