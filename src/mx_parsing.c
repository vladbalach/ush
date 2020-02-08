#include "ush.h"
// static int end_flag(char *str, int *position, int end, int flag);

// bool mx_check_symbol(char *str, int position, char symbol) {
//     int check = 0;

//     if (str[position] != symbol)
//         return 0;
//     while (position > 0 && str[position] != 0) {
//         if (str[--position] != 92)
//             position = 0;
//         else
//             check++;
//     }
//     if (check % 2 == 0)
//         return 1;
//     return 0;
// }

// static int flang_Comand(char *str, int *position, int end, int flag) {
//     // mx_printstr("\nComand\n");
//     int check_flag = flag;

//     while (*position < end) {
//         check_flag = str[*position];
//         if (mx_check_symbol(str, *position, flag))
//             return 0;
//         else if (mx_check_symbol(str, *position, 34) || mx_check_symbol(str, *position, 96) || mx_check_symbol(str, *position, 39)) {
//             (*position)++;
//             check_flag = end_flag(str, position, end, check_flag);
//         }
//         else if (position > 0 && str[*position] == 40 && mx_check_symbol(str, *position - 1, '$')){
//             (*position)++;
//             check_flag = end_flag(str, position, end, ')');
//         }
//         else
//             (*position)++;
//     }
//     mx_printerr("u$h: parse error near | ");
//     mx_printcharerr(flag);
//     mx_printerr(" |\n");
//     return flag;
// }

// static int flang_String(char *str, int *position, int end, int flag) {
//     // mx_printstr("\nString\n");
//     while (*position < end) {
//         if (mx_check_symbol(str, *position, 39))
//             return 0;
//         (*position)++;
//     }
//     mx_printerr("u$h: parse error near | \' |\n");
//     return flag;
// }


// static int end_flag(char *str, int *position, int end, int flag) {
//     int check_flag = 0;

//     // mx_printstr("\n");
//     // mx_printchar(flag);
//     // mx_printstr("\n");
//     if (flag == 39)
//         check_flag = flang_String(str, position, end, flag);
//     else
//         check_flag = flang_Comand(str, position, end, flag);
//     (*position)++;
//     return check_flag;
// }


// static int syntax_analiz(char *temp, int *position, int end) {

// }

void mx_parsing(char *str) {
    int end = mx_strlen(str);
    char *temp = mx_strdup(str);
    int position = 0;
    int flag = 0;
    char **comands = 0;

    while (position <= end) {
        if (flag != 0)
            flag = mx_end_flag(temp, &position, end, flag);
        if (position < end) {
            if (mx_check_symbol(temp, position, ';')) {
                if (temp[position + 1] == ';') {
                    mx_printerr("u$h: parse error near \';;\'\n");
                    position = end;
                    flag = 1;
                }
                else
                    temp[position] = 0;
            }
            if (mx_check_symbol(temp, position, 34) || mx_check_symbol(temp, position, 39) || mx_check_symbol(temp, position, 96))
                flag = temp[position];
            if (position > 0 && temp[position] == 40 && mx_check_symbol(temp, position - 1, '$'))
                flag = ')';
        }
        position++;
    }
    if (flag != 0) {
        mx_strdel(&temp);
        return;
    }
    comands = mx_create_comands(temp, end);
    // if (flag == 0)
        // mx_printstr("\n\nOK!!\n\n");
         mx_printstr("\n");
    for(int i = 0;  comands[i]; i++) {
        mx_printstr(comands[i]);
        mx_printstr("\n");
    }
}
