#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <term.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include "libmx.h"
#include <time.h>

// VARIABLES

typedef struct s_variable {
    char* name;
    char* value;
} t_variable;

typedef struct s_tree_node {
    struct s_tree_node *left;
    struct s_tree_node *right;
    struct s_tree_node *parent;
    
    void *data;
} t_tnode;

enum e_branch {
    RIGHT,
    LEFT
};

enum e_pipe_status {
    PIPE_NOTHING,
    PIPE_R,
    PIPE_W,
    PIPE_RW
};

// AST

enum e_type_of_token{
    TYPE_OPERATOR,
    TYPE_COMMAND,

    TYPE_FILENAME,
    TYPE_DESC_NUMB,
};

typedef struct s_token{
    char type;
    char **value;
    int priority;
} t_token;

typedef struct s_programInfo {
    struct termios term_old;
    struct termios term_new;
} t_info;

enum e_keys{
    CTRL_A = 1,
    CTRL_W = 23,
    CTRL_E = 5,
    CTRL_R = 18,
    CTRL_T = 20,
    CTRL_Y = 25,
    CTRL_U = 21,
    CTRL_I = 9, // TAB
    CTRL_P = 16,
    CTRL_CLOSE_BRACKET = 29,
    CTRL_D = 4,
    CTRL_F = 6,
    CTRL_G = 7, // sound
    CTRL_H = 8,
    CTRL_K = 11, // vertical tab
    CTRL_L = 12, // new feed
    CTRL_Z = 26,
    CTRL_X = 24,
    CTRL_C = 3,
    CTRL_B = 2,
    CTRL_N = 14,

    KEY_ESCAPE = 27,
    KEY_SPACE = 32,
    KEY_ENTER = 13
};


// tree
void mx_insert_tree(t_tnode **root, t_tnode *new, 
    int (*cmp)(void*, void*),
    void (*free_)(void *)
);
t_tnode* mx_get_min_tnode(t_tnode *root);
t_tnode *mx_create_tnode(void *data);
void mx_delete_tnode(t_tnode **root, void *data, int (*cmp)(void*, void*), void (*free_tnode)(t_tnode *tnode));
t_tnode *mx_find_tnode(t_tnode *root, void *data, int (*cmp)(void*, void*));

t_token *mx_create_token(char type, char **value, int priority);
void mx_clear_tokens(t_list **tokens);
t_token* mx_get_next_token(int *start, int end, char *str);
char mx_get_token_type(char *str);
// 
void mx_ush_init();
void mx_init();
void mx_parsing(char *str, char **envp);
char **mx_key_tab(char *parsing);
void mx_key_duble_tab(char **str, char **comands, int *table);
void mx_print_Tab_comands(t_list *list_comand);
t_list *mx_lexer(char *str);
bool mx_syntax_analyzer(t_list *tokens);
void mx_execute(char **commands);

typedef struct termios t_termios;
//BUILT IN
void mx_cd(char *str);
void mx_printstr_env(char *str);
void mx_pwd();
void mx_echo(char **str);
void mx_env(char *envp[]);
void mx_export(const char *str, char **envp);

//
bool mx_input(t_list **list_comands);
void out_monitor(int position, char *str, int count, int ch);
void clean_monitor(char *str, int *table, char *new_str);

// lexer
bool mx_is_char(char c);

// AST
t_tnode* mx_create_ast(t_list** tokens, t_tnode *prev);
void mx_delete_ast(t_tnode **root);

//exec
void mx_execute_tree(t_tnode *root, int *fds, char pipeStatus);


#endif
