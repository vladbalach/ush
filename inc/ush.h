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
#include <sys/wait.h>

// #define HISTORY_STRING "\x1b[38;2;2;2;2mu$h> \x1b[0m\x1b[33m"
#define MAX_PROC_COUNT 500
#define HISTORY_STRING "\x1b[38;5;243mu$h> \x1b[38;5;68m"
// #define MAIN_STRING "\x4u$h> "
#define NAME "\x4\x1b[38;5;76mu$h> \x1b[38;5;76m"
#define SEARCH "\x8\x1b[38;5;243mSearch > \x1b[38;5;68m"
// #define SEARCH_NAME_REMOVE "\x8Search > "

// VARIABLES

typedef struct s_variable {
    char *name;
    char *value;
    bool is_env;
    char *mem;
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
    RIGHT_VISITED = 64,
    OP_AMPERSAND = 32
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

typedef struct s_process {
    pid_t pid;
    int index;
} t_process;

typedef struct s_input {
    char **comands;
    char **comand_tab;
    int id;
    int max_comand;
    int str_len;
    int end_posit;
    int if_;
    int pos_tab;
} t_input;

typedef struct s_programInfo {
    struct termios term_old;
    struct termios term_new;
    char **env;
    t_list *history;
    t_input *input;
    t_list *processes;
    bool isExit;
    int exit_status;
    char *pwd;
    char *pwdL;
    char *pwdP;
    char *old_pwd;
    char *path;
    char *home;
    t_list *var_tree;
    int lastStatus;
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
void mx_if_new_parameter(char *str, int *start, int end, t_info *processes);
char *mx_return_value(char **str, t_list **var_tree);
void mx_serch_list(t_list **var_tree, char *name, char *value);
t_token *mx_create_token(char type, char **value, int priority);
void mx_clear_tokens(t_list **tokens);
t_token* mx_get_next_token(int *start, int end, char *str, t_info *processes);
char mx_get_token_type(char *str);

// 
void mx_ush_init(t_info **info, char **env);
void mx_parsing(char *str, t_info *info);
t_list *mx_lexer(char *str, t_info *processes);
bool mx_syntax_analyzer(t_list *tokens);
void mx_execute(char **commands, t_info *processes);
void mx_ush_close(t_info *info);

void mx_write_from_to(int from , int to, off_t start);
typedef struct termios t_termios;

//BUILT IN
int mx_cd(char **argv, t_info *info);
void mx_printstr_env(char *str);
int mx_pwd(char **argv, t_info *info);
void mx_echo(char **str);
void mx_env(char **argv, t_list *var_tree);
void mx_export(char **argv, t_list **var_tree);
void mx_unset(char **argv, t_list **var_tree);
void mx_which(char **argv, t_info *info);
bool mx_is_buildin(char *str);

//
bool mx_check_symbol(char *str, int position, char symbol);
int mx_end_flag(char *str, int *position, int end, int flag);
char *mx_parsing_input(char *str);
char mx_if_isspace(char s);
char *mx_audit_str(char *str, t_info *processes, bool dqute);
char *mx_str_bquote(char **str, t_info *processes);
char **mx_create_comands(char *str, int end);
void mx_one_symbol(char **str, char ch, int *count, int position);
void mx_not_ascii(char *chars, int *table,  char **comands);
int mx_bit_sumbol(char *str);
int mx_len_symbol(int sum, char *str);
int mx_input(t_info *info);
void mx_print_esc(char *s);
void mx_check_outprogram_new_line(void);
void mx_clean_monitor(char *str, int *table, char *new_str);
void mx_out_monitor_new(char *name, int table2, int pos,char *str);
void mx_clean_monitor_new(char *name, int table2, int pos,char *str);
void mx_print_esc(char *s);
void mx_key_delite(char **comands, int *table);
char **mx_key_tab(char *parsing, int *table, char **str, t_info *info);
void mx_key_duble_tab(char **str, char **comands, int *table);
void mx_print_Tab_comands(t_list *list_comand);
t_info* mx_get_info(t_info *info);


// lexer
bool mx_is_char(char c);
int mx_replace_bquote(char **str, t_info *info);
void mx_HOME(char **str, int *i, t_info *processes);
void mx_do_replace(char **str, size_t start, size_t end, char *str_new);

// AST
t_tnode* mx_create_ast(t_list** tokens, t_tnode *prev);
void mx_delete_ast(t_tnode **root);

//exec
int mx_execute_tree(t_tnode *root, int *fds, char operatorStatus, t_info *info);
int mx_exec_more(t_tnode *root, int *fds, int operatorStatus, t_info *info);
int exec_token(t_token *token, int *fds, char operatorStatus, t_info *info);
void mx_exec_less(t_tnode *root, int *fds, char operatorStatus, t_info *info);
void mx_execute_proces(t_token* token);
void mx_close_all_pr(t_info *info);
int mx_pipe_execute(t_tnode *root, int *fds, char operatorStatus, t_info *processes);

// processes
int mx_add_process(t_list **processes, pid_t pid);

#endif
