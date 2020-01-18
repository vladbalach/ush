#include "ush.h"

static void delete_no_child(t_tnode **root, void *data, int (*cmp)(void*, void*)) {
    t_tnode *root_ = *root;
    int result = cmp(root_->data, data);

    if (result == 0) {
        // free *root
        *root = 0;
    }
    else if (result > 0) {
       delete_no_child(&((*root)->left), data, cmp); 
    }
    else {
        delete_no_child(&((*root)->right), data, cmp); 
    }
}

static void delete_tnode_1ch(t_tnode **root, void *data, int (*cmp)(void*, void*)) {
    t_tnode *root_ = *root;
    int result = cmp(root_->data, data);
    if ((root == 0) || (*root == 0))
        return;
    if (result == 0) {
        if ((*root)->right != 0)
            *root = (*root)->right;
        else if ((*root)->left != 0)
            *root = (*root)->left;
    }
    else if (result > 0) {
        delete_tnode_1ch(&((*root)->left), data, cmp);
    }
    else {
        delete_tnode_1ch(&((*root)->right), data, cmp);
    }
}

static void delete_tnode_2ch(t_tnode **root, void *data, int (*cmp)(void*, void*), t_tnode *finded) {

    t_tnode *min = mx_get_min_tnode(finded->right);
    printf("str = %s\n", ((t_variable*)min->data)->name);
    mx_delete_tnode(root, min->data, cmp);
    finded->data = min->data;
    
}

void mx_delete_tnode(t_tnode **root, void *data, int (*cmp)(void*, void*)) {
    t_tnode *finded = mx_find_tnode(*root, data,  cmp);
    
    if (finded == 0) {
        return;
    }
    if (finded == *root) {
        // delete_root();
    }
    if (finded == 0) { // no value
        return;
    }
    if ((finded->left == 0) && (finded->right == 0)) {
        printf("NO CHILD\n");
        delete_no_child(root, data, cmp);
    }
    else
    if ((finded->left == 0) && (finded->right != 0)) {
        printf("1 RIGHT\n");
        delete_tnode_1ch(root, data, cmp);
    }
    else
    if ((finded->left != 0) && (finded->right == 0)) {
        printf("1 LEFT\n");
        delete_tnode_1ch(root, data, cmp);
    }
    else
    if ((finded->right != 0) && (finded->left !=0)) {
        printf("2 CHILD\n");
        delete_tnode_2ch(root, data, cmp, finded);
    }
}
