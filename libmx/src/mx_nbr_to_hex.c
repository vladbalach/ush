#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    unsigned long ac = nbr;
    int a = 1;
    char *s = NULL;

    while (ac > 16) {
        a++;
        ac = ac / 16;
    }
    s = malloc(sizeof(char) * (a + 1));
    s[a--] = '\0';
    while (a >= 0) {
        if ((nbr - (nbr / 16) * 16) < 10) 
            s[a] = (nbr - (nbr / 16) * 16) + '0';
        else 
            s[a] = (nbr - (nbr / 16) * 16) + 'a' - 10;
        nbr = nbr / 16;
        a--;
    }
    return s;
}
