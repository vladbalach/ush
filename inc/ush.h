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

#include "libmx.h"

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

typedef struct termios t_termios;
//BUILT IN
void mx_cd(char *str);
void mx_printstr_env(char *str);
void mx_pwd();
void mx_echo(char **str);
void mx_env(char *envp[]);
void mx_export(const char *str, char **envp);
bool mx_input(t_list **list_comands);
void out_monitor(int position, char *str, int count, int ch);
void clean_monitor(char *str, int *table, char *new_str);
// 

#endif
