#include "ush.h"
#include "macroses.h"

// static char *read_to_delim(int des, int des2) {
//     int count = 1;
//     char buff[2];
//     char *new_str = 0;

//     buff[1] = 0;
//     close(des2);
//     while((count = read(des, buff, 1))) {
//         if (buff[0] == '\n')
//             buff[0] = '\x0d';
//         new_str = mx_strjoin2(new_str, buff);
//     }
//     if (new_str) {
//         count = strlen(new_str);
//         if (new_str[count] == '\x0d')
//             new_str[count] = 0;
//         if (count > 1 && new_str[count - 1] == '\x0d')
//             new_str[count - 1] = 0;
//     }
//     close(des);
//     return new_str;
// }

char *mx_strjoin4(char *s1, char *s2) {
    // char *s = NULL;
    size_t a;


    if (s1 != 0 && s2 != 0) {
    size_t a1 = strlen(s1);
    size_t a2 = strlen(s2);
    s1 = realloc(s1, a1 + a2 + 1);
        // s = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
        a = a1;
        // if (s != 0){
        //     for (int i = 0 ; i < a; i++)
        //         s[i] = s1[i];
            for (size_t i = 0; i < a2; a++, i++)
                s1[a] = s2[i];
        // }
        s1[a] = 0;
        // free(s1);
        return s1;
    }
    if (s1 == 0 && s2 == 0)
        return 0;
    if (s1 != 0)
        return mx_strdup(s1);
    return mx_strdup(s2);
}



static char *read_to_delim(int des, int des2) {
    char *str = 0;
    char *temp = malloc(1025 * sizeof(char));
    // char *temp2 = 0;
    ssize_t check;
    int i = -1;

    close(des2);
    while ((check = read(des, temp, 1024)) != 0) {
        // temp2 = mx_strndup(temp, check);
        temp[check] = 0;
        str = mx_strjoin4(str, temp);
    }
    free(temp);
    while (str && str[++i] != 0)
        if (str[i] == '\n')
            str[i] = '\x0d';
    if (str) {
        i = strlen(str);
        if (str[i - 1] == '\x0d')
            str[i - 1] = 0;
    //     if (i > 1 && str[i - 2] == '\x0d')
    //         str[i - 2] = 0;
    }
    close(des);
    return str;
}

static void child(t_info *processes, int des[2], char **str) {
    close(des[0]);
    dup2(des[1], 1);
    mx_parsing(*str, processes);
    if (processes->last_status == 130 || !(processes->if_ctrl_c))
        exit(130);
    exit(0);
}

static void parent(t_info *processes, int status, char **str) {
    int check = MX_EXSTATUS(status);
    mx_strdel(str);
    // close(des[1]);
    if (check == 130) {
        processes->if_ctrl_c = 0;
        // close(des[0]);
    }
}

char *mx_str_bquote(char **str, t_info *processes) {
    int des[2] = {0, 0};
    pid_t pid;
    int status;
    char *test = 0;

    pipe(des);
    pid = fork();
    if (pid == 0)
        child(processes, des, str);
    else if (pid == -1) {
        mx_printerr(strerror(errno));
        exit(1);
    }
    else
        test = read_to_delim(des[0], des[1]);
    wait(&status);
    parent(processes, status, str);
    if (!processes->if_ctrl_c)
        return 0;
    // mx_printerr("\n\nkkk\n");
    return test;
}
