#include "ush.h"

static void delete_no_child(t_tnode **root, void *data,
                            int (*cmp)(void*, void*),
                            void (*free_tnode)(t_tnode *tnode)) {
    t_tnode *root_ = *root;
    int result = cmp(root_->data, data);

    if (result == 0) {
        free_tnode(*root);
        *root = 0;
    }
    else if (result > 0) {
       delete_no_child(&((*root)->left), data, cmp, free_tnode);
    }
    else {
        delete_no_child(&((*root)->right), data, cmp, free_tnode);
    }
}

static void delete_tnode_1ch(t_tnode **root, void *data,
                             int (*cmp)(void*, void*),
                             void (*free_tnode)(t_tnode *tnode)) {
    t_tnode *del_node = 0;

    if ((root == 0) || (*root == 0))
        return;
    if (cmp((*root)->data, data) == 0) {
        if ((*root)->right != 0) {
            del_node = *root;
            *root = (*root)->right;
            free_tnode(del_node);
        }
        else if ((*root)->left != 0) {
            del_node = *root;
            *root = (*root)->left;
            free_tnode(del_node);
        }
    }
    else if (cmp((*root)->data, data) > 0)
        delete_tnode_1ch(&((*root)->left), data, cmp, free_tnode);
    else
        delete_tnode_1ch(&((*root)->right), data, cmp, free_tnode);
}

static void delete_tnode_2ch(t_tnode **root, int (*cmp)(void*, void*),
                             t_tnode *finded,
                             void (*free_tnode)(t_tnode *tnode)) {
    t_tnode *min = mx_get_min_tnode(finded->right);

    mx_delete_tnode(root, min->data, cmp, free_tnode);
    finded->data = min->data;
}

void mx_delete_tnode(t_tnode **root, void *data, int (*cmp)(void*, void*),
                     void (*free_tnode)(t_tnode *tnode)) {
    t_tnode *finded = mx_find_tnode(*root, data,  cmp);

    if (finded == 0)
        return;
    if (finded == *root) {
        printf("ROOTTT\n");
        free_tnode(finded);
    }
    if (finded == 0) // no value
        return;
    if ((finded->left == 0) && (finded->right == 0))
        delete_no_child(root, data, cmp, free_tnode);
    else if ((finded->left == 0) && (finded->right != 0))
        delete_tnode_1ch(root, data, cmp, free_tnode);
    else if ((finded->left != 0) && (finded->right == 0))
        delete_tnode_1ch(root, data, cmp, free_tnode);
    else if ((finded->right != 0) && (finded->left !=0))
        delete_tnode_2ch(root, cmp, finded, free_tnode);
}
