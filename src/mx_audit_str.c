#include "ush.h"


static void substitution_comand(char **str, char **str2, t_info *processes) {
    mx_strdel(str);
    for (int i = 0; str2[0][i]; i++) 
        if (str2[0][i] == '\\' && (str2[0][i + 1] == '`'
            || str2[0][i + 1] == '\\'))
            mx_do_replace(str2, i, i + 1, 0);
    *str2 = mx_str_bquote(str2, processes);
    *str = *str2;
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
    else if (str[0][0] == 96 || str[0][0] == '$')
        substitution_comand(str, &temp, processes);
    else if (str[0][0] == 34) { 
        temp2 = mx_audit_str(temp, processes, 1);
        mx_subs(&temp2);
        mx_strdel(str);
        mx_strdel(&temp);
        *str = temp2;
    }
}

static bool chek_comand(char *new_str, int i) {
    if (new_str[i] == 34 || new_str[i] == 39 || new_str[i] == 96) {
        if (i == 0 || new_str[i - 1] != '\''
            || !mx_check_symbol(new_str, i, new_str[i]))
            return true;
        else
            return false;
    }
    else if (new_str[i + 1] == '(' && new_str[i] == '$'
             && (i == 0 || new_str[i - 1] != '\''))
        return true;
    else if (new_str[i + 1] == '(' && new_str[i - 1] == '\''
             && !mx_check_symbol(new_str, i, '$'))
        return true;
    else
        return false;
}

static void spec_symbol(t_info *processes, int *i, char **new_str) {
    int flag = new_str[0][*i];
    int pos = *i + 1;
    char *comand = 0;

    if (new_str[0][i[0]] == '$') {
        pos++;
        flag = ')';
    }
    mx_end_flag(new_str[0], &pos, strlen(new_str[0]), flag);
    comand = mx_strndup(&new_str[0][i[0]], pos - i[0]);
    editor_str(&comand, processes);
    mx_do_replace(new_str, i[0], pos, comand);
    if (comand) {
        (i[0]) += mx_strlen(comand);
        mx_strdel(&comand);
    }
    (i[0])--;
}

char *mx_audit_str(char *str, t_info *processes, bool dqute) {
    char *new_str = 0;
    int i = 0;
    int pos = 0;

    if (str == 0)
        return 0;
    new_str = mx_strdup(str);
    for (; new_str && new_str[i]; i++) {
        pos = i;
        if (new_str[i] == '~' && !dqute)
            mx_HOME(&new_str, &i, processes);
        else if (chek_comand(new_str, i))
            spec_symbol(processes, &i, &new_str);
        else if (mx_check_symbol(new_str, i ,'$'))
            mx_parametr_shell(processes, &i, &new_str);
        else if (new_str[i] == '\\'
                 && (!dqute || (dqute && (new_str[i + 1] == '\\'))))
            mx_do_replace(&new_str, i, i + 1, 0);
    }
    return new_str;
}
