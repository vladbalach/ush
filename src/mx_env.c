#include "ush.h"

/* 
* -1 - not flagline
*/
static int get_flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    if (str[i + 1] == '\0') 
        return 4; // - flag
    while(str[++i]) {
        if (str[i] == 's') 
            flags |= 1;
        else if (str[i] == 'P') 
            flags |= 2;
        else {
            return -1;
        }
    }
    return flags;
}

/* 
* 1-st bit -> -i
* 2-nd bit -> -P
* 3-rd bit -> -u
*/
static int get_flags(char **argv, int *i) {
    int flags = 0;

    while(argv[++(*i)]) {
        if (argv[(*i)][0] != '-')
            return flags;
        int curr = get_flags_from_line(argv[*i]);
        if (curr == -1)
            return 0;
        flags |= curr;
    }
    i--;
    return flags;
}

void mx_env(char **argv, t_list *var_tree) {
    t_list *var_tree_temp = var_tree;

    while (var_tree_temp) {
        if (((t_variable*)var_tree_temp->data)->is_env) {
            mx_printstr(((t_variable*)var_tree_temp->data)->mem);
            write(1,"\n",1);
        }
        var_tree_temp = var_tree_temp->next;
    }
}
