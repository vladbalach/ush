#include "ush.h"

t_tnode *mx_find_tnode(t_tnode *root, void *data, int (*cmp)(void*, void*)) {
    t_tnode *finded = 0;
    if (root == 0) {
        return 0;
    }
    if (cmp(root->data, data) == 0) {
        finded = root;
    }
    else if (cmp(root->data, data) < 0) {
        if (root->right == 0) {
            return 0;
        }
        else {
            finded = mx_find_tnode(root->right, data, cmp);
        }
    }
    else {
        if (root->left == 0) {
            return 0;
        }
        else {
            finded = mx_find_tnode(root->left, data, cmp);
        }
    }
    return finded;
}
