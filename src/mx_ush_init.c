#include "ush.h"

void mx_segfault() {
    mx_printerr("OOPS! SEGMENT FAULT\n");
    exit(1);
}
void mx_ctrl_c() {
    // t_info *info = mx_get_info(0);
    // t_list *tmp = info->processes;
    // pid_t pid = 0;
    // while(tmp) {
    //     pid = (pid_t) tmp->data;
    //     kill(pid, SIGKILL);
    // }
    printf("CTRL+C\n");
}


static void init_var() {
    // int initCount = 10;
    
    // *variables = (t_variable **) malloc (sizeof(t_variable*) * initCount); // initCount -vars
    // for (int i = 0; i < initCount; i++) {
    //     (*variables)[i] = (t_variable *) malloc (sizeof(t_variable));
    // }
}

static int cmp(void *a1, void* a2) {
    return mx_strcmp(((t_variable*)a1)->name,
        ((t_variable*)a2)->name);


}

static void free_(void* a) {
    t_variable *t = (t_variable*)a;
    free(t->name);
    free(t->value);
    free(t);
    
}

static void free_del(t_tnode *a) {
    t_variable *t = a->data;
    // free(a);
    free(t->name);
    free(t->value);
    free(t);
}

static int cmp_del(void *data_root, void*data) {
    return mx_strcmp(((t_variable*)data_root)->name,
        (char*)data);
}

static void start_program(t_tnode **var_tree, char **env) {
    int i = -1;
    char **envvar = 0;
    t_variable *var = 0;
    t_tnode *node = 0;
    printf("A\n");
    // node = (t_tnode*) malloc( sizeof(t_tnode));
    // node->data = mx_strdup("a");
    // mx_insert_tree(&var_tree, node, cmp ,free_);
    while (env[++i]) {
        
        // envvar = mx_strsplit(env[i], '=');
        var = (t_variable*) malloc(sizeof(t_variable));
        node = (t_tnode*) malloc( sizeof(t_tnode));
        var->name = mx_strdup("asd");
        var->value = mx_strdup("asd");;
        var->is_env = true;
        node->data = var;
        printf("B\n");
        mx_insert_tree(var_tree, node, cmp ,free_);
        printf("C\n");

        // mx_del_strarr(&envvar);
    }
     
    printf("name = %s\n",((t_variable*)(*var_tree)->data)->name);
    // while (var_tree) 
        // mx_delete_tnode(&var_tree, "XPC_FLAGS", cmp_del,free_del );
        // printf("name = %s\n",((t_variable*)var_tree->right->data)->name);
}

void mx_ush_init(t_info **info, char **env) {
    t_info *newInfo = (t_info*) malloc(sizeof(t_info));
    newInfo->env = env;
    newInfo->processes = (pid_t*) malloc(sizeof(pid_t) * 11);
    char *buff = 0;
    int i = 0;
    for(i = 0; i < 10; i++)
        newInfo->processes[i] = 0; // empty
    newInfo->isExit = false;
    newInfo->exit_status = 0;
    newInfo->path = mx_strdup(getenv("PATH"));
    newInfo->pwd = getcwd(NULL, 0);
    newInfo->pwdP = getcwd(NULL, 0);
    newInfo->pwdL = getcwd(NULL, 0);
    newInfo->old_pwd = getcwd(NULL, 0);
    //  printf("pwd = %s\n", newInfo->old_pwd);
    newInfo->home = mx_strdup(getenv("HOME"));
    (*info) = newInfo;
    start_program(&(newInfo->var_tree), env);
    // mx_get_info(*info);
    signal(SIGSEGV, mx_segfault);
    signal(SIGINT, mx_ctrl_c);
}
