#include "ush.h"

bool mx_check_symbol(char *str, int position, char symbol) {
    int check = 0;

    if (str[position] != symbol)
        return 0;
    while (position > 0 && str[position] != 0) {
        if (str[--position] != 92)
            position = 0;
        else
            check++;
    }
    if (check % 2 == 0)
        return 1;
    return 0;
}
