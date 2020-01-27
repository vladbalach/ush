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
#include <signal.h>

// #define HISTORY_STRING "\x1b[38;2;2;2;2mu$h> \x1b[0m\x1b[33m"
#define HISTORY_STRING "\x1b[38;5;243mu$h> \x1b[38;5;68m"
#define MAIN_STRING "u$h> "
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

enum e_operator_status {
    OP_NOTHING,
    OP_PIPE_R,
    OP_PIPE_W,
    OP_PIPE_RW,
    OP_MORE = 4,
    OP_DMORE = 8,
    OP_LESS = 16,
    LEFT_VISITED = 128,
    RIGHT_VISITED = 64
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
t_list *mx_lexer(char *str);
bool mx_syntax_analyzer(t_list *tokens);
void mx_execute(char **commands);

void mx_write_from_to(int from , int to, off_t start);
typedef struct termios t_termios;
//BUILT IN
void mx_cd(char *str[]);
void mx_printstr_env(char *str);
void mx_pwd(char *str);
void mx_echo(char **str);
void mx_env(char *envp[]);
void mx_export(const char *str, char **envp);

//
void mx_one_symbol(char **str, char ch, int *count, int position);
void mx_not_ascii(char *name, char *chars, int *table,  char **comands);
int mx_bit_sumbol(char *str);
int mx_len_symbol(int sum, char *str);
int mx_input(t_list **list_comands);
void mx_print_esc(char *s);
void mx_clean_monitor(char *str, int *table, char *new_str);
void mx_out_monitor_new(char *name, int table2, int pos,char *str);
void mx_clean_monitor_new(char *name, int table2, int pos,char *str);
void mx_print_esc(char *s);
void mx_key_delite(char *name, char **comands, int *table);
char **mx_key_tab(char *parsing, int *table, char **str);
void mx_key_duble_tab(char **str, char **comands, int *table);
void mx_print_Tab_comands(t_list *list_comand);

// lexer
bool mx_is_char(char c);

// AST
t_tnode* mx_create_ast(t_list** tokens, t_tnode *prev);
void mx_delete_ast(t_tnode **root);

//exec
void mx_execute_tree(t_tnode *root, int *fds, char pipeStatus);
void mx_exec_more(t_tnode *root, int *fds, int operatorStatus);
void mx_exec_token(t_token* token, int *fds, char pipe_status);
void mx_exec_less(t_tnode *root, int *fds, char operatorStatus);
void mx_execute_proces(t_token* token);
#endif
