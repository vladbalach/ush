#include "libmx.h"

char *mx_itoa(int number) {
    char a[12];
    long long b;
    int i = 0;

    if (number == 0) {
        a[0] = '0';
        i++;
    }
    if ((b = (long long)number) < 0)
        b = - b;
    while (b > 0) {
        a[i++] = b % 10 + '0';
        b /= 10;
    }
    if (number < 0)
        a[i++] = '-';
    a[i] = 0;
    mx_str_reverse(a);
    return mx_strdup(a);
}
