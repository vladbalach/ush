#include "ush.h"

// static void printerr(name) {
//     mx_printerr("ush: ");
//     mx_printerr(argv[1]);
//     mx_printerr(": ");
//     mx_printerr(strerror(errno));
//     mx_printerr("\n");
//     exit(1);
// }

// static void flags(char **argv, t_list *var_tree) {
//     pid_t childpid = fork();

//     if (childpid == -1) {
//         exit(1)
//     }
//     else if (childpid == 0) {
//         if (flag == 1) { // -i
//             if (execvpe(/*назва програми */, argv, 0) == -1) {
//                 printerr(name);
//             }
//         }
//         if else (flag == 2) { // -P
//             if (execve(/*шлях до програми*/, argv, /*env*/) == -1)
//                 printerr(name);
//         }
//         if else (flag == 3) { // -u
//             if (execve(/*назва програми*/, argv, /*env*/) == -1)
//                 printerr(name);
//         }
//     }
//     else {
//         wait();
//     }
//     exit(EXIT_SUCCESS);
// }
/* 
*  -u не передає в процес заданий аргумент env
*  -P вказати шлях до програм які ти хочеш виконувати
*  -і не передавати env в процес 
*/ 
static bool reg(char *str, char *regular) {
    regex_t regex;
    int result;

    regcomp(&regex, regular, REG_EXTENDED);
    result = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return result == 0 ? 1 : 0;
}

// static t_var *var_tree_to_var(t_list *var_tree) {
//     t_var *var = malloc(sizeof(t_var));
//     t_var *save = var;

//     var->next = NULL;
//     var->value = NULL;
//     for (;var_tree; var_tree = var_tree->next) {
//             if (var->value == NULL && ((t_variable*)var_tree->data)->is_env) {
//                 var->value = strdup(((t_variable*)var_tree->data)->mem);
//                 var->flag = 1;
//             }
//             else if (((t_variable*)var_tree->data)->is_env) {
//                 var->next = malloc(sizeof(t_var));
//                 var = var->next;
//                 var->flag = 1;
//                 var->value = strdup(((t_variable*)var_tree->data)->mem);
//                 var->next = NULL;
//         }
//     }
//     return save;
// }

void mx_env(char **argv, ) {
    t_var *var = var_tree_to_var(info->var_tree);
    // char *path = NULL;
    char *regular_verenv = "^.+=.*$";
    char *regular_i = "^-i+$";
    char *regular_u_up = "^-u+.*$";
    char *regular_p_up = "^-P+.*$";
    char *reg_p = "^-P$";
    char *reg_u = "^-u$"; 
    char *reg_name_prog = "^a-z+$"; 
    int i = 0;

    for (i = 1; argv[i] != NULL; i++) {
        if (reg(argv[i], regular_p)) {
            // get_path(argv[i], argv[i + 1], &path, &i);
            // flag[0] = 1;
            printf("flag P %s\n", argv[i]);
        }
        else if (reg(argv[i], regular_u)) {
            // get_delete_veriable(argv[i], argv[i + 1], var, &i);
            // flag[1] = 1;
            printf("flag u %s\n", argv[i]);
        }
        else if (reg(argv[i], regular_i)) {
            // flag[2] = 1;
            printf("flag i %s\n", argv[i]);
        }
        else if (reg(argv[i], regular_verenv))
            break;
        else if (reg(argv[i], reg_name_prog))
            break;
    }
    printf("%s\n", ((t_variable*)var_tree->data)->mem);
    // if (argv[i] == NULL) {
        // print_var(var);
    // }
    // for (;argv[i] != NULL; i++) {

    // }
    // char **delete = NULL;
    // int flag = 0;
    // int i = 1;
    // int mileston = 0
    // int count = 0;

    // for (int n = 0; argv[n + 1]; n++) {
    //     mileston = n + 1;
    //     if (strcmp(argv[n], "-P") != 0 && strcmp(argv[n], "-u") != 0
    //         && strcmp(argv[n + 1], "-P") != 0 && strcmp(argv[n + 1], "-u") != 0)
    //         break;
    // }
    // argv++;
    // for (int n = 0; n < mileston; n++)
    //     if (strcmp(argv[n], "-u") == 0) {
    //         if (argv[n + 1] == NULL) {
    //             error_env("u")
    //             exit(1);
    //         }   
    //         else 
    //             i++;
    //     }
    // if (i > 1)
    //     flag++;
    // delete = malloc(sizeof(char *) * i);
    // delete[i] = NULL;
    // for (int n = 0; n < mileston, n++) {
    //     if (strcmp(argv[n], "-u") == 0) {
    //         delete[count] = strdup(argv[n + 1]);
    //         count++;
    //     }
    // }
    // for (int n = 0; n < mileston; n++) {
    //     if (strcmp(argv[n], "-P") == 0) {
    //         if (argv[n + 1] == NULL) {
    //             error_env("P");
    //             exit(1);
    //         }
    //         else {
    //             free(path);
    //             path = mx_strdup(argv[n + 1])
    //         }
    //     }
    // }


    // if (path != NULL)
    //     flag += 2;
    // for (int n = 0; n < mileston; n++) {
    //     if (strcmp(argv[n], "-i") == 0) {
    //         flag += 4;
    //         break;
    //     }
    // }
    // int counter = 0;
    // int lenargv = 0;
    // for (; argv[counter]; counter++) {
    //     if (strcmp(argv[counter], "-i") == 0 && argv[counter + 1] != NULL 
    //         && argv[counter + 1][0] != '-') {
    //         break;
    //     }
    //     else if (argv[counter][0] != '-' && argv[counter + 1] != NULL 
    //               && argv[counter][0] != '-') {
    //         break;
    //     }
    // }
    // for (;argv[counter]; counter++, lenargv++);
    // char *argvexec = malloc(sizeof(char *) * (lenargv) + 1);
}
