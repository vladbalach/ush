#include "ush.h"

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
        mx_strdel(str);
        for (int i = 0; temp[i]; i++) 
            if (temp[i] == '\\' && (temp[i + 1] == '`' || temp[i + 1] == '\\'))
                mx_do_replace(&temp, i, i + 1, 0);
        temp = mx_str_bquote(&temp, processes);
        *str = temp;
    }
    else if (str[0][0] == 34) {
        temp2 = mx_audit_str(temp, processes, 1);
        mx_strdel(str);
        mx_strdel(&temp);
        *str = temp2;
    }
}

static bool is_not_operator(char c) {
    if (c == '|' || c == '&' || c == '>' || c == '<'|| c == '$' || c == ' ')
        return false;
    if (c == 34 || c == 39 || c== 96 || c == 0 || c== '?')
        return false;
    if (c == '='|| c == 92)
        return false;
    return true;
}

static int end_parametr(char *str, int i) {
    int temp = i + 1;

    while (is_not_operator(str[temp]))
        temp++;
    if (str[temp] == '?' && str[temp + 1] != '(')
        temp++;
    return temp;
}

// static bool if_symbol(char test) {
//     if (test == 34 || test == 39 || test == 96)
//         return 1;
//     return 0;
// }
static bool chek_comand(char *new_str, int i) {
    if (new_str[i] == 34 || new_str[i] == 39 || new_str[i] == 96) {
        if (i == 0 || new_str[i - 1] != '\'' || !mx_check_symbol(new_str, i, new_str[i]))
            return true;
        else
            return false;
    }
    else if (new_str[i + 1] == '(' && (i == 0 || new_str[i - 1] != '\''  || !mx_check_symbol(new_str, i, '$')))
        return true;
    else
        return false;
}

char *mx_audit_str(char *str, t_info *processes, bool dqute) {
    if (str == 0)
        return 0;
    char *new_str = mx_strdup(str);
    char *temp = NULL;
    int flag = 0;
    int i = 0;
    int pos = 0;

    for (; new_str && new_str[i]; i++, pos = i) {
        if (new_str[i] == '~' && !dqute)
            mx_HOME(&new_str, &i, processes);
        // else if ((if_symbol(new_str[i]) && (i == 0 || new_str[i - 1] != '\'' || !mx_check_symbol(new_str, i, new_str[i])))
        //     || (new_str[i + 1] == '(' && (i == 0 || new_str[i - 1] != '\''  || !mx_check_symbol(new_str, i, '$')))) {
        else if (chek_comand(new_str, i)) {
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
            mx_do_replace(&new_str, i, pos, temp);
            if (temp) {
                i = i + mx_strlen(temp);
                mx_strdel(&temp);
            }
            i--;
        }
        else if (mx_check_symbol(new_str, i ,'$')) {
            flag = end_parametr(new_str, i);
            temp = mx_strndup(&new_str[i + 1], flag - i - 1);
            if (temp[0] == '?') {
                mx_strdel(&temp);
                temp = mx_itoa(processes->lastStatus);
            }
            else
                temp = mx_return_value(&temp, &(processes->var_tree));
            mx_do_replace(&new_str, i, flag, temp);
            if (temp)
                i = i + mx_strlen(temp);
            i--;
        }
        else if (new_str[i] == '\\' && (!dqute || (dqute && (new_str[i + 1] == '\\'))))
            mx_do_replace(&new_str, i, i + 1, 0);
    }
    return new_str;
}
