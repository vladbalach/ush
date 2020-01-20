#include "ush.h"

static t_list* find_max_priority(t_list* tokens) {
    t_list *tmp = tokens;
    t_list* maxNode = tmp;
    if (tmp == 0)
        return 0;
    int maxPriority = ((t_token*)tmp->data)->priority;
    printf("PRIORITET:");
    while (tmp) {
        printf("%d ", ((t_token*)tmp->data)->priority);
        if (((t_token*)tmp->data)->priority > maxPriority) {
            maxPriority = ((t_token*)tmp->data)->priority;
            maxNode = tmp;
        }
        tmp = tmp->next;
    }
    printf("\n");
    return maxNode;
}

static void delete_max_from_list(t_list **tokens, t_list *maxToken) {
    t_list *tmp = *tokens;
    if (tmp == 0)
        return;
    if (tmp == maxToken) {
        *tokens = 0;
        return;
    }
    while (tmp->next) {
        if (tmp->next == maxToken) {
            tmp->next = 0;
            break;
        }
        tmp = tmp->next; 
    }
}

t_tnode* mx_create_ast(t_list* tokens) {
    // sleep(1);
    t_tnode *root = 0;
    t_list *maxToken = find_max_priority(tokens);
    t_list *newStart = 0;
    if (maxToken == 0)
        return 0;
    root = (t_tnode*) malloc (sizeof(t_tnode));
    newStart = maxToken->next;
    root->data = maxToken;
    delete_max_from_list(&tokens, maxToken);
    
    if (tokens != 0)
        root->left = mx_create_ast(tokens);
    if (newStart != 0)
        root->right = mx_create_ast(newStart);
    
    return root;
}
