#include "ush.h"
// signals 

// void mx_init(t_info* mainInfo) {
//     signal(SIGINT, mx_sig_int);
//     signal(SIGKILL, mx_sig_kill);
//     // set parametrs for terminal
//     setTerminal(mainInfo);
// }

// void mx_close(t_info *mainInfo, char **str) {
//     free(*str);
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &mainInfo->term_old);
// }

// // input
// // 1 - end of input ( \n ; )

static void escape_sequences(char* chars, int *position, int count) {
    if (chars[2] == 65) { // UP
        printf("\x1B[32;1mUP ARROW\x1B[0;1m\n");
    }
    if (chars[2] == 66) { // DOWN
        // printf("\033[2;2H");
         //  write(1, chars, 3);
        printf("\x1B[33;1mDOWN ARROW\x1B[0;0m\n");
    }
    if (chars[2] == 67) { // RIGHT
        if (0 < *position) {
            write(1, chars, 3);
            (*position)--; 
            
        }
        // printf("\x1B[34;1mRIGHT ARROW\x1B[0;1m\n");
    }
    if (chars[2] == 68) { // LEFT
        if (count - 1 > *position) {
            write(1, chars, 3);
            (*position)++;
           
        }
        // printf("\x1B[31;1mLEFT ARROW\x1B[0;1m\n");
    }
}

int mx_handleEvents(char ch) {
    if (ch == KEY_ENTER) {
        write(1, "\n", 1);
        return KEY_ENTER;
    }
    if (ch == CTRL_C || ch == CTRL_Z || ch == CTRL_D) {
        return -1; // exit
    }
    return 0;
}

void one_symbol(char **str, char ch, int *count, int position) {
    int i = 0;

    if (ch == 127) {
        if ((*count) > 1) {
            (*count)--;
            i = position;
            while (i > 0) {
                (*str)[(*count) - i - 1] = (*str)[(*count) - i];
                i--;
            }
            (*str)[(*count) - 1] = 0;
        }
    }
    else {
        write(1, &ch, 1);
        (*count)++;
        *str = realloc(*str, *count);
        while (i <= position) {
            (*str)[(*count) - i - 1] = (*str)[(*count) - i - 2];
            i++;
        }
        (*str)[(*count) - position - 2] = ch;
    }
}

void out_monitor(int position, char *str) {
    struct winsize w;
    int ws_col = 0;
    int temp = mx_strlen(str) - position + 6;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ws_col = temp / w.ws_col;
    for (int i = ws_col; i > 0; i--)
        write(1,"\n",1);
    write(1,"\r",1);
    if (ws_col == 0)
    mx_printstr("u$l = ");
    mx_printstr(&str[ws_col * w.ws_col]);
    mx_printstr(" ");
    temp = position;
    while (temp-- != -1)
        write(1, "\b", 1);
}

char* mx_input() {
    unsigned int ch = 0;
    char *chars = 0;
    char *str = mx_strnew(0);
    int count = 1;
    int position = 0;
    // str[0] = 0;
    while(1) {
        if((ch = mx_getchar()) == 0)
            mx_printerr("uSh: some troubeles with input!\n");
        if (ch > 127) { // 2-4 symbols
            chars = (char*)(&ch);
            if (chars[0] == 27) {
               escape_sequences(chars, &position, count);
            }
        }
        else { // 1 symbol
            if (ch < 32) {
                int action = mx_handleEvents(ch);
                if (action == -1) { // CTRL_C | Z
                    free(str);
                    return 0;
                }
                if (action == KEY_ENTER) {
                    return str;
                }
            }
            else {
                one_symbol(&str, ch, &count, position);
            }
        }
        out_monitor(position, str);
        // int i = position;
        // write(1,"\r",1);
        // mx_printstr("u$l = ");
        // mx_printstr(str);
        // mx_printstr(" ");
        // while (i-- != -1)
        //     write(1, "\b", 1);
    }
}

void mx_parsing(char *str) {

}

void mx_do_actions(char *str) {

}

int main(int argc, char *argv[], char *envp[]) {
    char *str = 0;

    while(1) {
        mx_printstr("u$l = ");
        str = mx_input();
        if (str == 0)
            goto end_l;
        mx_printstr("str = ");
        mx_printstr(str);
        mx_printstr("|\n");
        mx_parsing(str);
        mx_do_actions(str);
        mx_strdel(&str);
    }
    end_l:
    // system("leaks ush");
    return 0;
}

