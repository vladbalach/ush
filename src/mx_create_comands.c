#include "ush.h"
static char check_flag(char str, char flag) {
    char temp = flag;

    if (temp == 0)
        temp = str;
    else if (temp == str)
        temp = 0;
    return temp;
}

static void str_spli_comands(char ***comands, char *str, int len_str) {
    char **temp = *comands;
    char flag = 0;
    int start = 0;
    int i = 0;
    int namber = 0;

    while (i < len_str && str[i] != 0) {
        while (str[i++] == ';');
        start = --i;
        while (str[i] != 0 && flag != 1) {
            if (str[i] == ';' && (i > 0 && str[i - 1] != 92) && flag == 0 && i--) {
                flag = 1;
                // i--;
            }
            if (str[i] == 44 || str[i] == 34 || str[i] == 96)
                flag = check_flag(str[i], flag);
            i++;
        }
        flag = 0;
        if (i - start != 0)
            temp[namber++] = mx_strndup(&str[start], i - start);
        if (str[i] != 0)
            i++;
    }
    *comands = temp;
}

char **mx_create_comands(char *str) {
    int words = mx_count_words(str, ';');
    char **comands = (char **) malloc ((words + 1) * sizeof(char *));
    int len_str = mx_strlen(str);

    if (comands != 0) {
        for (int i = 0; i <= words; i++)
            comands[i] = 0;
        str_spli_comands(&comands, str, len_str);
    }
    for (int i = 0; comands[i] != 0; i++) {
        mx_printstr("\n\n|");
        mx_printstr(comands[i]);
        mx_printstr("|\n\n");
    }
    return comands;
}
