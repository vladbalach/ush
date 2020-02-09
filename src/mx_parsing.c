// #include "ush.h"
// static bool check_symbol(char *str, int position, char symbol) {
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

// static int end_flag(char *str, int *position, int end, int flag) {

// }


void mx_parsing(char *str) {
    // char *temp = mx_strdup(str);
    // int position = 0;
    // int end = mx_strlen(str);
    // int flag = 0;

    // while (position < end) {
    //     if (flag != 0)
    //         flag = end_flag(temp, &position, end, flag);
    //     if (position < end) {
    //         if (check_symbol(temp, position, ';')) {
    //             if (temp[position + 1] == ';') {
    //                 mx_printerr("u$h: parse error near \';;\'\n");
    //                 position = end;
    //             }
    //             else
    //                 temp[position] = 0;
    //         }
    //         if (check_symbol(temp, position, 4) || check_symbol(temp, position, 34) || check_symbol(temp, position, 96))
    //             flag = temp[position];
    //         if (position > 0 && temp[position] == '(' && check_symbol(temp, position - 1, '$'))
    //             flag = temp[position];
    //     }
    //     position++;
    // }
}
