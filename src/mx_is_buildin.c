#include "ush.h"

bool mx_is_buildin(char *str) {
    if (str == 0)
        return 0;
    if ((mx_strcmp(str, "cd") == 0)
        || (mx_strcmp(str, "pwd") == 0)
        || (mx_strcmp(str, "env") == 0)
        || (mx_strcmp(str, "which") == 0)
        || (mx_strcmp(str, "echo") == 0)
        || (mx_strcmp(str, "export") == 0)
        || (mx_strcmp(str, "unset") == 0)
        || (mx_strcmp(str, "fg") == 0)
        || (mx_strcmp(str, "exit") == 0))
        return true;
    return false;
}
