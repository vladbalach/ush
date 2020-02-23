#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    int a = 0;
    unsigned long sum = 0;
    unsigned long pow = 1;

    while (hex[a])
        a++;
    for (int i = a - 1; i >= 0; i--) {
        if(hex[i] >= '0' && hex[i] <= '9')
            sum = sum + (hex[i] - '0') * pow;
        if(hex[i] >= 'A' && hex[i] <= 'Z')
            sum = sum + (hex[i] - 'A' + 10) * pow;
        if(hex[i] >= 'a' && hex[i] <= 'z')
            sum = sum + (hex[i] - 'a' + 10) * pow;
        pow *= 16;
    }
    return sum;
}
