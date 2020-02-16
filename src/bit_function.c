#include "ush.h"

int mx_bit_sumbol(char *str) {
    unsigned char temp = (unsigned char)str[0];

    if (temp >> 7 == 0)
        return 1;
    if (temp >> 5 == 6)
        return 2;
    if (temp >> 4 == 14)
        return 3;
    if (temp >> 3 == 30)
        return 4;
    return 0;
}

int mx_len_symbol(int sum, char *str) {
    unsigned char temp;
    int sumator = 0;

    for (int i = 0; i < sum; i++) {
        temp = (unsigned char)str[i];
        if (temp >> 7 == 0)
            sumator++;
        else if (temp >> 6 == 3)
            sumator++;
        if (temp >> 3 == 30)
            sumator++;
    }
    return sumator;
}
