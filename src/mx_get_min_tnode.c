#include "ush.h"

t_tnode* mx_get_min_tnode(t_tnode *root) {
    t_tnode *min = 0;

    if (root == 0) 
        return 0;
    if (root->left == 0) {
        min = root;
    }
    else {
        min = mx_get_min_tnode(root->left);
    }
    return min;
}
