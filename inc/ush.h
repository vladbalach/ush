#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdlib.h>
#include <xlocale.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <term.h>
#include <sys/types.h>
#include <regex.h> 
#include <pwd.h>
#include <stdio.h>
#include <dirent.h>
#include "libmx.h"
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <termcap.h>


#define MX_STR info->input->comands[info->input->id]
#define MX_COMMAND info->input->comands
#define MX_ID info->input->id
#define MX_STR_LEN info->input->str_len
#define MX_STR_POS info->input->end_posit
#define MX_MAX_COMAND info->input->max_comand
#define MX_SYMBOL info->input->if_
#define MX_ID_TAB_KEY info->input->pos_tab
#define MX_FLAGS_W O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD
#define MX_FLAGS_WE O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD
#define MAX_PROC_COUNT 500
#define MX_HISTORY_STRING "\x1b[38;5;243mu$h> \x1b[38;5;68m"
#define MX_NAME "\x4\x1b[38;5;76mu$h> \x1b[38;5;76m"
#define MX_SEARCH "\x8\x1b[38;5;243mSearch > \x1b[38;5;68m"
#define MX_PATH ((t_token*)tmp->next->next->data)->value[0]
#define MX_FUNC_RETURN mx_return_value("HOME", &(info->var_tree))
#define MX_GET_PATH (argv[i] ? argv[i] : MX_FUNC_RETURN)

#define MX_REG_EXPORT   "^[A-Za-z_]+[A-Za-z_0-9]*(=.*)?$"
#define MX_REG_ERR      "^-(i+)?[^Pui]+$"
#define MX_REG_I        "^-i+((P|u)?|((P|u).+)?)$"
#define MX_REG_U        "^-(i+)?u.*$"
#define MX_REG_P        "^-(i+)?P.*$"
#define MX_REG_VER      "^.+=.*$"
#define MX_REG_PROG     "^[^-]+$"

// VARIABLES

typedef struct s_var {
    char *name;
    char *value;
    bool flag;
    struct s_var *next;
} t_var;

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
    OP_AMP = 32
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
    char **name;
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
void mx_start_program(t_list **var_tree, char **env);
void mx_delete_tnode(t_tnode **root, void *data, int (*cmp)(void*, void*), void (*free_tnode)(t_tnode *tnode));
void mx_push_env(t_list **var_tree, char *name, char *value, char *mem);
t_tnode *mx_find_tnode(t_tnode *root, void *data, int (*cmp)(void*, void*));
void mx_if_new_parameter(char *str, int *start, int end, t_info *processes);
char *mx_return_value(char **str, t_list **var_tree);
char *mx_return_value2(const char *str, t_list **var_tree);
void mx_serch_list(t_list **var_tree, char *name, char *value);
t_token *mx_create_token(char type, char **value, int priority);
void mx_clear_tokens(t_list **tokens);
t_token* mx_get_next_token(int *start, int end, char *str, t_info *processes);
char mx_get_token_type(char *str);
void mx_parametr_shell(t_info *processes, int *i, char **new_str);
int mx_flang_Comand(char *str, int *pos, int end, int flag);
void mx_read_user(char **user);
t_token *mx_token_in_program(int *currPos, int end, char *str, t_info *processes);
int mx_exec_dmore(t_tnode *root, int *fds, int operatorStatus, t_info *info);
void mx_ush_init(t_info **info, char **env);
void mx_subs(char **str);
void mx_parsing(char *str, t_info *info);
t_list *mx_lexer(char *str, t_info *processes);
bool mx_syntax_analyzer(t_list *tokens);
void mx_execute(char **commands, t_info *processes);
void mx_ush_close(t_info *info);
void mx_charge_parametr_export(char *value, char *tmp, t_variable *data);

void mx_write_from_to(int from , int to, off_t start);
// typedef struct termios t_termios;

//BUILT IN
int mx_cd(char **argv, t_info *info);
int mx_history(t_list **list_comands);
void mx_printstr_env(char *str);
int mx_pwd(char **argv, t_info *info);
void mx_echo(char **str,  t_info *info);
void mx_env(char **argv, t_info *info);
void mx_export(char **argv, t_list **var_tree, t_info *info);
void mx_unset(char **argv, t_list **var_tree, t_info *info);
void mx_which(char **argv, t_info *info);
bool mx_is_buildin(char *str);
void mx_jobs(t_info *info);
void mx_fg(char **argv, t_info *info);
void mx_exit(t_token *token, t_info *info);

//CD 
int mx_chdir_P(char *path, t_info *info, char flags);
char* mx_add_one_rank(char *path, char *new_part);
char* mx_del_last_rank(char *path);
int mx_chdir_L(char *path, t_info *info, char flags);

// Which
bool mx_is_commad(char *fullname, int flags);

//
bool mx_check_symbol(char *str, int position, char symbol);
void mx_ctrl_v_and_not_ascii(t_info *info, char *chars);
void mx_ctrl_R(t_info *info);
int mx_ascii(t_info *info, char *chars, unsigned int ch);
int mx_end_flag(char *str, int *position, int end, int flag);
char *mx_parsing_input(char *str);
char *mx_audit_str(char *str, t_info *processes, bool dqute);
char *mx_str_bquote(char **str, t_info *processes);
char **mx_create_comands(char *str, int end);
void mx_one_symbol(char **str, char ch, int *count, int position);
void mx_not_ascii(char *chars, t_info *info);
int mx_bit_sumbol(char *str);
int mx_len_symbol(int sum, char *str);
int mx_input(t_info *info);
void mx_print_esc(char *s);
void mx_check_outprogram_new_line(void);
void mx_clean_monitor(char *str, t_info *info, char *new_str);
void mx_print_esc(char *s);
void mx_out_monitor_new(char *name, int table2, int pos,char *str);
void mx_clean_monitor_new(char *name, int table2, int pos,char *str);
void mx_print_esc(char *s);
void mx_key_delite(t_info *info);
char **mx_key_tab(char *parsing, char **str, t_info *info);
char *mx_mini_parser_tab(char *parsing, t_info *info);
void mx_read_comand(char *parsing, t_list **list_comand);
void mx_key_duble_tab(char **str, char **comands, t_info *info);
void mx_print_Tab_comands(t_list *list_comand);
t_info* mx_get_info(t_info *info);
bool mx_is_link(char *file);
void mx_unset_fds(int *fds, int *savedFds, int operator_starus);
t_var *mx_var_tree_to_var(t_list *var_tree);
int mx_get_twidth();

// lexer
bool mx_is_char(char c);
void mx_HOME(char **str, int *i, t_info *processes);
void mx_do_replace(char **str, size_t start, size_t end, char *str_new);
bool mx_is_operator(t_token *token);

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
int mx_buildin_list(t_token *token, t_info *info);
void mx_exec_env_pr(char *path, char **argv, char **env, t_info *info);

// processes
int mx_add_process(t_list **processes, pid_t pid, char **name);
void mx_del_top_process(t_info *info);
void mx_del_pid_process(t_info *info, int pid);
void mx_wait_process(t_info *info, char **argv);
void mx_segfault();
void mx_ctrl_c();
void mx_ctrl_z();
void mx_segfault_in();

//print
void mx_print_susp(char **mas_name);
void mx_print_cont(char **mas_name, int pid);
char **mx_get_name(t_info *info, int numb);

// env 

char **mx_call_vlad(char **argv, int i);
char **mx_env_to_vlad(t_var *var);
bool mx_check_env(char **argv, char **path, t_var *var, int *i);
bool mx_print_error_env(char *str, int flag);
void mx_print_env(t_var *var);
void mx_fre_env_path(t_var *var, char *path);
bool mx_reg(char *str, char *regular);
char *mx_get_path_env(char *str1, char *str2, int *i);
int mx_delete_veriable_env(char *str1, char *str2, t_var *var, int *i);

#endif
