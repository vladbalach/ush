#include "ush.h"

static bool if_symbol(char test) {
    if (test == 34 || test == 39 || test == 96)
        return 1;
    return 0;
}

// static char if_isspace2(char s) {
//     char temp = s;

//      if (s == 0)
//         temp = '0';
//     else if (s == '\a')
//         temp = 'a';
//     else if (s == '\b')
//         temp = 'b';
//     else if (s == '\t')
//         temp = 't';
//     else if (s == '\n')
//         temp = 'n';
//     else if (s == '\v')
//         temp = 'v';
//     else if (s == '\f')
//         temp = 'f';
//     else if (s == '\r')
//         temp = 'r';
//     return temp;
// }

// static char if_isspace2(char s) {
//     char temp = s;

//     if (s == '0')
//         temp = 0;
//     else if (s == 'a')
//         temp = '\a';
//     else if (s == 'b')
//         temp = '\b';
//     else if (s == 't')
//         temp = '\t';
//     else if (s == 'n')
//         temp = '\n';
//     else if (s == 'v')
//         temp = '\v';
//     else if (s == 'f')
//         temp = '\f';
//     else if (s == 'r')
//         temp = '\r';
//     return temp;
// }

// char mx_if_isspace(char s) {
//     char temp = s;

//     if (s < 15 && s > -1)
//         temp = if_isspace2(s);
//     else if (s == 48 || s > 96 && s < 19)
//         temp = if_isspace3(s);
//     return temp;
// }

static void del_backslash_and_$(char **str, t_info *processes) {
    char *new_str = 0;
    char temp[2];
    int i = 0;

    temp[1] = 0;
    while (str[0][i] != 0) {
        if (str[0][i] == 92)
            temp[0] = mx_if_isspace(str[0][++i]);
        else
            temp[0] = str[0][i];
        new_str = mx_strjoin2(new_str, temp);
        i++;
    }
    mx_strdel(str);
    *str = new_str;
}

static void editor_str(char **str, t_info *processes) {
    char *temp = 0;
    char *temp2 = 0;
    int i = 1;

    if (str[0][0] == '$')
        i++;
    temp = mx_strndup(&str[0][i], mx_strlen(*str) - i - 1);
    if (str[0][0] == 39) {
        mx_strdel(str);
        *str = temp;
    }
    else if (str[0][0] == 96 || str[0][0] == '$') {
        // del_backslash_and_$(&temp, processes);
        temp = mx_str_bquote(&temp, processes);
        mx_strdel(str);
        *str = temp;
    }
    else if (str[0][0] == 34) {
        temp2 = mx_audit_str(temp, processes, 1);
        // del_backslash_and_$(&temp2, processes);
        mx_strdel(str);
        mx_strdel(&temp);
        *str = temp2;
    }
}

static void do_replace(char **str, size_t start, size_t end, char *str_new) {
    char *newStr = NULL;;
    int i = mx_strlen(*str);
    int j = end;
    int sum = 0;

    if (str_new)
        sum = mx_strlen(str_new);
    if (!(sum ==  0 && i == end - start))
    newStr = (char*) malloc (i - (end - start) + sum);
    if (newStr) {
        for (i = 0; i < start; i++) {
            newStr[i] = (*str)[i];
        }
        while(sum != 0 && str_new[i - start]) {
            newStr[i] = str_new[i - start];
            i++;
        }
        while((*str)[j])
            newStr[i++] = (*str)[j++];
        newStr[i] = 0;
    }
    free(*str);
    *str = newStr;
    // mx_printstr(newStr);
    // mx_printstr("||\n");
}

static bool is_not_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<'|| c == '$' || c == ' ')
        return false;
    if (c == '='|| c == 92 || c == 34 || c == 39 || c== 96 || c == 0)
        return false;
    return true;
}

static int end_parametr(char *str, int i) {
    int temp = i + 1;

    while (is_not_operator(str[temp]))
        temp++;
    return temp;
}

char *mx_audit_str(char *str, t_info *processes, bool dqute) {
    char *new_str = mx_strdup(str);
    char *temp = NULL;
    int pos = 0;
    int flag = 0;

    // mx_printstr();
    for (int i = 0, pos = 0; new_str && new_str[i]; i++, pos = i) {
        if ((if_symbol(new_str[i]) && mx_check_symbol(new_str, i, new_str[i]))
            || (new_str[i + 1] == '(' && mx_check_symbol(new_str, i, '$'))) {
            pos++;
            if (new_str[i] == '$') {
                pos++;
                flag = ')';
            }
            else
                flag = new_str[i];
            mx_end_flag(new_str, &pos, mx_strlen(new_str), flag);
            temp = mx_strndup(&new_str[i], pos - i);
            editor_str(&temp, processes);
            do_replace(&new_str, i, pos, temp);
            if (temp) {
                i = i + mx_strlen(temp);
                mx_strdel(&temp);
            }
            i--;
        }
        else if (mx_check_symbol(new_str, i ,'$')) {
            flag = end_parametr(new_str, i);
            temp = mx_strndup(&new_str[i + 1], flag - i);
            temp = mx_return_value(&temp, &(processes->var_tree));
            do_replace(&new_str, i, flag, temp);
            if (temp)
                i = i + mx_strlen(temp);
            i--;
        }
        else if (new_str[i] == '\\' && (!dqute || (dqute && (new_str[i + 1] == '\\')))) {
            do_replace(&new_str, i, i + 1, 0);
        }
    }
    // mx_printstr(new_str);
    return new_str;
}